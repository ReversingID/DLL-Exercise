/*
Inject sebuah DLL ke sebuah proses

Compile:
    $ g++ injector.cpp -o injector

Run:
    $ injector <PID> <dll path>
*/

#include <windows.h>
#include <stdio.h>

/*
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

int inject(const int pid, const char* dll_path);

int main(int argc, char* argv[])
{
    int choice = 1;

    if (argc != 3) 
    {
        fprintf(stderr, "[!] Should have 2 arguments!\n");
        fprintf(stderr, "Usage: %s <PID> <dll path>", argv[0]);
        return 1;
    }

    return inject(atoi(argv[1]), argv[2]);
}

int inject(const int pid, const char* dll_path)
{
    HANDLE remote;
    HANDLE remote_thread;
    void * alloc_addr;
    int    dll_path_len = strlen(dll_path) + 1;
    int    write_result;
    DWORD  tid;

    int    error_code;
    char   error_message_buffer[256];

    printf("[+] Starting DLL Injector\n");

    // [1] Open the remote process
    // API: HANDLE OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
    printf("[+] Get process handle for PID: %d\n", pid);
    remote = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (remote == NULL)
    {
        error_code = GetLastError();
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error_message_buffer, 256, NULL);
        fprintf(stderr, "[!] Cannot open the remote process!\nDetail: %d %s\n", error_code, error_message_buffer);
        return 1;
    }
    printf("    [=] Handle obtained: %p\n", remote);

    // [2] Allocating space in the process
    // API: LPVOID VirtualAllocEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
    printf("[+] Allocating space for DLL path\n");
    alloc_addr = VirtualAllocEx(remote, NULL, dll_path_len, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    if (alloc_addr == NULL)
    {
        CloseHandle(remote);

        error_code = GetLastError();
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error_message_buffer, 256, NULL);
        fprintf(stderr, "[!] Cannot allocate memory on remote process!\nDetail: %d %s\n", error_code, error_message_buffer);
        return 2;
    }
    printf("    [=] Address allocated: %p\n", alloc_addr);

    // [3] Write DLL path on remote process
    // API: BOOL WriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesWritten);
    printf("[+] Writing DLL path to current process space\n");
    SIZE_T num_written;
    write_result = WriteProcessMemory(remote, alloc_addr, dll_path, dll_path_len, &num_written);
    if (! write_result)
    {
        VirtualFreeEx(remote, alloc_addr, 0, MEM_RELEASE);
        CloseHandle(remote);

        error_code = GetLastError();
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error_message_buffer, 256, NULL);
        fprintf(stderr, "[!] Cannot write to process memory!\nDetail: %d %s\n", error_code, error_message_buffer);
        return 3;
    }
    printf("    [=] Writing success!\n");

    // [4] Resolve address of kernel32.dll & LoadLibraryA function
    // API: HMODULE GetModuleHandleA(LPCSTR lpModuleName);
    printf("[+] Resolving call specific functions and libraries\n");
    HMODULE kernel_handle_addr = GetModuleHandleA("kernel32.dll");
    printf("    [=] Resolved kernel32 library at 0x%08x\n", kernel_handle_addr);

    // API: FARPROC GetProcAddress(HMODULE hModule,LPCSTR lpProcName);
    void * load_lib = (LPVOID) GetProcAddress(kernel_handle_addr, "LoadLibraryA");
    printf("    [=] Resolved LoadLibraryA function at 0x%08x\n", load_lib);

    // [5] Create new thread on remote target to execute LoadLibraryA with our DLL path as argument
    // API: HANDLE CreateRemoteThread(
    //              HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, 
    //              LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
    printf("[+] Creating Remote Thread to load our DLL\n");
    remote_thread = CreateRemoteThread(remote, NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(load_lib), alloc_addr, 0, &tid);
    printf("    [=] Spawning thread with ID: %d\n", tid);
    if (remote_thread == NULL)
    {
        VirtualFreeEx(remote, alloc_addr, 0, MEM_RELEASE);
        CloseHandle(remote);

        error_code = GetLastError();
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error_message_buffer, 256, NULL);
        fprintf(stderr, "[!] Cannote create remote thread!\nDetail: %d %s\n", error_code, error_message_buffer);
        return 4;
    }

    // API: DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
    WaitForSingleObject(remote_thread, INFINITE);

    VirtualFreeEx(remote, alloc_addr, 0, MEM_RELEASE);
    CloseHandle(remote);

    return 0;
}
/*
Inject Multiple DLL to remote process

Compile:
    $ g++ multi-injector.cpp -o multi-injector

Run:
    $ multi-injector <PID> <dll path 1> .. <dll path N>
*/
#include <windows.h>
#include <iostream>
#include <memory>

/*
Idea:
    - Create new thread in remote / target process.
    - The new thread will execute our "DLL"

Steps:
    - Get the process handle for PID
    - Allocate space for string of DLL path
    - Write DLL path to the process selected in step 1
    - Resolve address of kernel32.dll & LoadLibraryA function
    - Create new thread on remote target to execute LoadLibraryA with our DLL path as argument

For another methods of injection, see "CodeInjection-Exercise" repository
*/

//======== Data Type Definitions ====================================

class Win32Handle 
{
    HANDLE internal;
public:
    Win32Handle() : internal(0)         { }
    ~Win32Handle()                      { CloseHandle(internal);   }
    HANDLE  get()                       { return internal; }
    HANDLE& operator=(HANDLE handle)    { internal=handle; return internal; }
    bool    operator==(HANDLE handle)   { return internal==handle; }
};

//======== Helper Functions =========================================

void   generate_message(const char* base_message); 
void * resolve_address(Win32Handle& process);
void * allocate(Win32Handle& process, const char* dll_path);
int    run_thread(Win32Handle& process, void * alloc_addr);
int    inject(Win32Handle& process, const char* dll_path);

//======== Start the Execution ======================================
int main(int argc, char* argv[])
{
    int i;
    int retval;
    int pid;
    Win32Handle process;

    if (argc < 3)
    {
        std::cerr << "[!] Should have at least 2 arguments!" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <PID> <DLL path 1> ... <DLL path N>" << std::endl;
        return 1;
    }

    pid = atoi(argv[1]);
    std::cout << "[+] Starting DLL injector" << std::endl << std::endl;
    std::cout << "[+] Get process handle for PID: " << pid << std::endl;

    // [1] Open the remote process and get the handle to it
    // API: HANDLE OpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
    process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (process == nullptr)
    {
        generate_message("Cannot open the remote process!");
        return 1;
    }
    std::cout << "    [=] Handle obtained: " << process.get() << std::endl << std::endl;

    for (i = 2; i < argc; i++)
    {
        retval = inject(process, argv[i]);
        if (retval)
            std::cerr << "[!] Stopping injection sequence" << std::endl;
    }
    return retval;
}

// Generate error message of Win32 API
void generate_message(const char* base_message)
{
    int   error_code = GetLastError();
    char  error_message_buffer[256];
    
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), error_message_buffer, 256, NULL);
    std::cerr << "    [!] " << base_message << std::endl;
    std::cerr << "        Detail: " << error_code << " " << error_message_buffer << std::endl;
}

// Resolve address of kernel32.dll & LoadLibraryA function
void * resolve_address(Win32Handle& process)
{
    static void * load_lib_addr = nullptr;

    if (load_lib_addr != nullptr)
        return load_lib_addr;

    std::cout << "    [+] Resolving call specific functions and libraries" << std::endl;
    
    // API: HMODULE GetModuleHandleA(LPCSTR lpModuleName);
    HMODULE kernel_handle_addr = GetModuleHandleA("kernel32.dll");
    std::cout << "        [=] Resolved! kernel32 library at " << kernel_handle_addr << std::endl;

    // API: FARPROC GetProcAddress(HMODULE hModule,LPCSTR lpProcName);
    load_lib_addr = (LPVOID) GetProcAddress(kernel_handle_addr, "LoadLibraryA");
    std::cout << "        [=] Resolved! LoadLibraryA function at " << load_lib_addr << std::endl;

    return load_lib_addr;
}

// Allocate space for DLL path
void* allocate(Win32Handle& process, const char* dll_path)
{
    void * alloc_addr;
    int    dll_path_len = strlen(dll_path) + 1;
    SIZE_T num_written;
    int    write_result;
    
    // [2] Allocating space in the process
    // API: LPVOID VirtualAllocEx(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
    std::cout << "    [+] Allocating space for DLL path" << std::endl;
    alloc_addr = VirtualAllocEx(process.get(), NULL, dll_path_len, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    if (alloc_addr == nullptr)
    {
        generate_message("Cannot allocate memory on remote process");
        return nullptr;
    }

    // [3] Write DLL path on rmeote process
    // API: BOOL WriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T * lpNumberOfBytesWritten);
    std::cout << "    [+] Writing DLL path to current process space" << std::endl;
    if (true)
    {
        write_result = WriteProcessMemory(process.get(), alloc_addr, dll_path, dll_path_len, &num_written);
        if (! write_result)
        {
            VirtualFreeEx(process.get(), alloc_addr, 0, MEM_RELEASE);
            generate_message("Cannot write to process memory!");
            return nullptr;
        }
    }
    std::cout << "        [=] Writing success!" << std::endl;
    return alloc_addr;
}

int run_thread(Win32Handle& process, void * alloc_addr)
{
    DWORD tid;
    Win32Handle thread;
    LPTHREAD_START_ROUTINE entry = (LPTHREAD_START_ROUTINE) resolve_address(process);

    // [5] Create new thread on remote target to execute LoadLibraryA with our DLL path as argument
    // API: HANDLE CreateRemoteThread(
    //              HANDLE hProcess, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, 
    //              LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
    std::cout << "    [+] Creating Remote Thread to load our DLL" << std::endl;
    thread = CreateRemoteThread(process.get(), NULL, 0, entry, alloc_addr, 0, &tid);
    if (thread == nullptr)
    {
        generate_message("Cannot create remote thread!");
        return 4;
    }
    std::cout << "        [=] Spawning thread with ID: " << tid << std::endl;

    // API: DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
    WaitForSingleObject(thread.get(), INFINITE);
    return 0;
}

int inject(Win32Handle& process, const char* dll_path)
{
    void * alloc_addr;
    int    retval;
    
    std::cout << "[*] Injecting DLL: " << dll_path << std::endl;

    alloc_addr = allocate(process, dll_path);
    if (alloc_addr == nullptr)
    {
        VirtualFreeEx(process.get(), alloc_addr, 0, MEM_RELEASE);
        return 2;
    }
    
    retval = run_thread(process, alloc_addr);
    VirtualFreeEx(process.get(), alloc_addr, 0, MEM_RELEASE);
    return retval;
}
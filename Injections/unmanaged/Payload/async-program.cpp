/*
    Sebagai korban untuk berbagai latihan dalam DLL injection.

    Asynchronous

Compile:
	(msvc)
	$ cl /nologo program.cpp library.lib

    (clang)
    x64
    $ clang++ -m64 program.cpp -o program.exe -L. -llibrary
    x86
    $ clang++ -m32 program.cpp -o program.exe -L. -llibrary

Run:
    $ program

*/

#include <stdio.h>
#include <string.h>
#include <process.h>
#include <windows.h>
#include <tlhelp32.h>

/*
Key points:
    - thread dapat mengeksekusi code secara asinkron dengan APC queues
    - setiap thread menyimpan semua APC

*/


//=== Forward Declarations ===========================================

DWORD WINAPI routine (LPVOID arg);

//=== M A I N ========================================================

int main()
{
    int i = 0;
    int choice = 0;
    int x, y;
    int pid = _getpid();
    int delay;

    HANDLE snapshot;
    HANDLE thread;
    HINSTANCE payload;
    THREADENTRY32 entry;
    ULONG  tid;
    DWORD  retval;

    printf ("My PID is %d\n\n", pid);
    printf ("[Thread %ld] starting ...\n", GetCurrentThreadId());

    do
    {
        printf ("Menu ----\n");
        printf ("1. List thread\n");
        printf ("2. Spawn a thread\n");
        printf ("3. Kill a thread\n");
        printf ("4. Unload payload.dll\n");
        printf ("0. Exit\n");
        printf ("Your choice? (1/2/3/4): ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 0:
                printf ("Exiting...");
                break;
            case 1:
                printf("[*] List of threads:\n\n");

                snapshot = CreateToolhelp32Snapshot (TH32CS_SNAPTHREAD, 0);
                if (snapshot != INVALID_HANDLE_VALUE)
                {
                    entry.dwSize = sizeof(THREADENTRY32);

                    if (Thread32First(snapshot, &entry))
                    {
                        do 
                        {
                            if (entry.th32OwnerProcessID != pid)
                                continue;
                            
                            printf("    [-] TID=%ld\n", entry.th32ThreadID);
                        } 
                        while (Thread32Next(snapshot, &entry));
                    }
                    CloseHandle(snapshot);
                }
                break;
            case 2:
                delay = 7;
                thread = CreateThread (NULL, 0, routine, &delay, 0, &tid);
                printf ("[*] Thread %ld started...\n", tid);
                break;
            case 3:
                printf("[*] Killing thread: ");
                scanf("%ld", &tid);

                thread = OpenThread(THREAD_ALL_ACCESS, FALSE, tid);
                if (thread == NULL)
                    printf("    [x] cannot open thread\n");
                else 
                {
                    if (!TerminateThread(thread, 0))
                        printf("    [v] thread killed\n");
                }
                break;
            case 4:
                payload = GetModuleHandleA("payload.dll");
                printf("payload.dll found at 0x%p\n", payload);
                FreeLibrary (payload);
                break;
            default:
                printf ("No such choice!\n");
                break;
        }

        printf ("\n\n");
    } while (choice != 0);
}

DWORD WINAPI routine (LPVOID arg)
{
    printf("[Thread %ld] starting\n", GetCurrentThreadId());

    while (true)
    {
        printf("[Thread %ld] running ...\n", GetCurrentThreadId());
        SleepEx(5000, TRUE);
    }

    printf("[Thread %ld] finish ...\n", GetCurrentThreadId());
}
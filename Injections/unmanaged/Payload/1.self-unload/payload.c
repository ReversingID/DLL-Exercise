/*
    Self Unload DLL
    Archive of Reversing.ID

    Inject DLL ini ke dalam process.
    Sebuah MessageBox akan tampil dengan pesan sesuai dengan state DLL.
    Unload DLL dengan menekan key tertentu pada console.

Compile:
    (msvc)
    $ cl /nologo /LD payload.c

    (clang)
    $ clang -shared payload.c -o payload.dll

    (gcc)
    $ gcc -shared payload.c -o payload.dll

Inject:
    $ injector <PID> payload.dll
*/

#include <windows.h>

#ifdef _MSC_VER
#pragma comment(lib,"advapi32")
#pragma comment(lib,"kernel32")
#pragma comment(lib,"user32")
#endif


HMODULE mainmod;

/*
    Thread khusus untuk memproses input.
    Alokasikan console baru untuk output
*/
DWORD WINAPI callback ()
{
    /* 
    secara asinkron mendapatkan key yang ditekan pada keyboard 

    daftar code virtual key
    https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    */
    while (1)
    {
        Sleep (100);
        if (GetAsyncKeyState (VK_CAPITAL))
            break;
    }

    /* bersihkan library dan keluar */
    Sleep (100);
    FreeLibraryAndExitThread (mainmod, 0);

    return 0;
}

BOOL WINAPI DllMain (HINSTANCE hinst, DWORD dwReason, LPVOID lpres)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            MessageBoxA(NULL, TEXT("DLL Injected! DllMain dipanggil dalam process attach."), TEXT("Salam"), MB_OK);

            mainmod = hinst;
            CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) callback, NULL, 0, NULL);
            break;
        case DLL_PROCESS_DETACH:
            MessageBoxA(NULL, TEXT("DllMain dipanggil dalam process detach."), TEXT("Salam"), MB_OK);
            break;
        case DLL_THREAD_ATTACH:
            MessageBoxA(NULL, TEXT("DllMain dipanggil dalam thread attach."), TEXT("Salam"), MB_OK);
            break;
        case DLL_THREAD_DETACH:
            MessageBoxA(NULL, TEXT("DllMain dipanggil dalam thread detach."), TEXT("Salam"), MB_OK);
            break;
    }
	return 1;
}
/*
    Simple Payload
    Archive of Reversing.ID

    Inject DLL ini ke dalam process.
    Sebuah MessageBox akan tampil dengan sebuah pesan sesuai dengan state DLL.

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

/*
Referensi: https://msdn.microsoft.com/en-us/library/windows/desktop/ms682583(v=vs.85).aspx

DLL_PROCESS_ATTACH:
    DLL sedang dimuat (load) ke dalam virtual address space dari sebuah proses.
DLL_PROCESS_DETACH:
    DLL sedang dilepas (unload) dari virtual address space dikarenakan:
        1. proses load yang gagal
        2. reference count telah mencapai nol.
            2.1 proses berakhir (baik secara sempurna maupun gagal)
            2.2 fungsi FreeLibrary() dipanggil (untuk setiap LoadLibrary() yang terjadi)
DLL_THREAD_ATTACH:
    Proses saat ini membuat sebuah thread baru dan terjadi pemanggilan ke setiap fungsi entry-point
    dari DLL yang ada pada proses.
DLL_THREAD_DETACH:
    Thread keluar secara sukses.
*/

#ifdef _MSC_VER
#pragma comment(lib,"advapi32")
#pragma comment(lib,"kernel32")
#pragma comment(lib,"user32")
#endif

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID lpres)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            MessageBoxA(NULL, TEXT("DLL Injected! DllMain dipanggil dalam process attach."), TEXT("Salam"), MB_OK);
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
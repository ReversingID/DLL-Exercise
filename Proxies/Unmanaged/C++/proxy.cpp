/*
    Ekspor dua fungsi untuk diimpor oleh Program.

Compile:
    (msvc)
    $ cl /LD proxy.cpp user32.lib
*/

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
    Implementasikan fungsi intercept() di Proxy kemudian forward setelah
    selesai eksekusi.
*/
void __declspec(dllexport) intercept (const char * message)
{
    MessageBox (NULL, message, "Proxy.Intercept()", MB_OK);
}

/*
    Forward pemanggilan fungsi forward() ke DLL target berdasarkan nama.
*/
#pragma comment(linker, "/export:forward=original.forward")

/*
    Untuk forward pemanggilan berdasarkan ordinal, gunakan

#pragma comment(linker, "/export:ord135=libtarget.#135,@135,NONAME")

    dengan 135 adalah ordinal yang diinginkan
*/


BOOL WINAPI _DllMainCRTStartup (HINSTANCE hinst, DWORD dwReason, LPVOID lpres)
{
    return 1;
}

#ifdef __cplusplus
}
#endif
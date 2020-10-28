/*
    Ekspor dua fungsi untuk diimpor oleh Program.

Compile:
    (msvc)
    $ cl /LD target.cpp user32.lib
*/

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

void __declspec(dllexport) intercept (const char * message)
{
    MessageBox (NULL, message, "Target.Intercept", MB_OK);
}

void __declspec(dllexport) forward (const char * message)
{
    MessageBox (NULL, message, "Target.Forward", MB_OK);
}

BOOL WINAPI _DllMainCRTStartup (HINSTANCE hinst, DWORD dwReason, LPVOID lpres)
{
    return 1;
}

#ifdef __cplusplus
}
#endif
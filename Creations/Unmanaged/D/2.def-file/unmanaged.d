/*
Pembuatan DLL sederhana dengan definition file.

Menggunakan def file untuk mendefinisikan DLL membuat penggunaan perintah `export`
menjadi tidak diperlukan.

Def file atau definition file adalah file yang digunakan untuk mendefinisikan
karakteristik dari objk executable / library yang dihasilkan.

Compile:
    [dmd]
        (x64)
        $ dmd -m64 -ofunmanaged.dll -shared unmanaged.d

        (x86)
        $ dmd -m32 -ofunmanaged.dll -shared unmanaged.d

Run:
    $ rundll32 unmanaged.dll
*/
module unmanaged;

import core.sys.windows.windows;
import core.sys.windows.dll;

// Exporting function world() 
extern(C)
void world()
{
    import core.stdc.stdio;
    printf("Hello from D\n");
}

// Exporting function calculate()
extern(C)
int calculate(int n)
{
    return n + 135;
}

version(Windows)
extern(Windows) 
bool DllMain(HINSTANCE hInstance, uint ulReason, LPVOID pvReserved)
{
    switch (ulReason)
    {
        default: assert(0);
        case DLL_PROCESS_ATTACH:
            dll_process_attach( hInstance, true );
            break;

        case DLL_PROCESS_DETACH:
            dll_process_detach( hInstance, true );
            break;

        case DLL_THREAD_ATTACH:
            dll_thread_attach( true, true );
            break;

        case DLL_THREAD_DETACH:
            dll_thread_detach( true, true );
            break;
    }
    return true;
}
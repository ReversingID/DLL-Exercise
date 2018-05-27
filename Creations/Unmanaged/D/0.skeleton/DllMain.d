/*
Pembuatan DLL standard dengan fungsi DllMain.

Perhatian: DLL ini tidak ekspor fungsi apapun!

Compile:
	[dmd]
        (x64)
        $ dmd -m64 -ofunmanaged.dll -shared DllMain.d

        (x86)
        $ dmd -m64 -ofunmanaged.dll -shared DllMain.d

    (llvm ldc)
    $ ldc

Run:
    $ rundll32 DllMain
*/
module unmanaged;

import core.sys.windows.windows;
import core.sys.windows.dll;

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
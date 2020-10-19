/*
Inject DLL ini ke dalam proses.
Pembacaan dan penulisan wilayah memory tertentu akan terjadi. Dalam kasus ini, 
nilai variabel global akan dimodifikasi.

Sesuaikan alamat variabel global sebelum melakukan kompilasi!!

Compile:
	[gcc]
		(x64)
		$ gcc -m64 -shared payload.c -o payload.dll
		
		(x86)
		$ gcc -m32 -shared payload.c -o payload.dll

	(msvc)
	$ cl /nologo /LD payload.c

Inject:
    $ injector <PID> payload.dll
*/
#include <windows.h>
#include <stdio.h>

// Lokasi variabel global
// Pastikan alamat ini sesuai
#define ADDRESS 0x404010

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID lpres)
{
    int* global = NULL;

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            global = (int*) ADDRESS;

            printf("\n\n");
            printf("Nilai variabel global = %d\n", *global);    // Read
            *global = 1337;                                     // Write
            printf("Nilai variabel global = %d\n", *global);
            break;
        case DLL_PROCESS_DETACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
	return 1;
}
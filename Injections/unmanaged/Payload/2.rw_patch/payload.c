/*
    RW Patch
    Archive of Reversing.ID

    Inject DLL ini ke dalam process.
    Pembacaan dan penulisan wilayah memory tertentu akan terjadi.
    Dalam kasus ini, nilai variabel global akan dimodifikasi.
    
    Sesuaikan alamat variabel global sebelum melakukan kompilasi.

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
#include <stdio.h>

/*
    Lokasi variabel global yang akan dibaca/ditulis.
    Pastikan alamat berikut sesuai.
*/
#define ADDRESS 0x404010

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID lpres)
{
    int* global = NULL;

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            global = (int*) ADDRESS;

            printf("\n\n");
            printf("Nilai awal  variabel global = %d\n", *global);      // Read
            *global = 1337;                                             // Write
            printf("Nilai akhir variabel global = %d\n", *global);
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
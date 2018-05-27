/*
Pembuatan DLL tak standard dengan entrypoint _DllMainCRTStartup()

Compile:
	[gcc]
		(x64)
		$ g++ -m64 -shared -o unmanaged.dll DllMainCRTStartup.cpp -Wl,--out-implib,libDllMainCRTStartup.a

		(x86)
		$ g++ -m32 -shared -o unmanaged.dll DllMainCRTStartup.cpp -Wl,--out-implib,libDllMainCRTStartup.a
	
	[msvc]
	$ cl /Feunmanaged.dll /LD DllMainCRTStartup.cpp

Run:
	$ rundll32 unmanaged.dll
*/
#include <windows.h>

// Nonaktifkan C++ mangle agar tidak ada dekorasi di simbol yang akan diekspor DLL.
#ifdef __cplusplus
extern "C" {
#endif

// Deklarasi fungsi yang diekspor oleh DLL
/*
__declspec(dllexport) void function(int arguments)
{ 
	
}
*/

/*
	DLL Entry point
	Umumnya, fungsi ini adalah entrypoint dan akan menginisialisasi C Run TIme.
	Ketika pembuatan DLL ditangani oleh compiler, fungsi ini akan memanggil fungsi DllMain() yang
	disertakan oleh programemr, jika ada.
	Fungsi DllMain() dibuat oleh pengguna dan bertujuan untuk menginisialisasi logic di sisi user.

	Jika kita membuat sendiri fungsi _DllMainCRTStartup, kita dapat membuat DLL minimum tanpa ada CRT.

	Jika perlu melakukan sesuatu dengan proses attach / detach maka gunakan DllMain().
	
	Reference: https://github.com/MicrosoftDocs/cpp-docs/blob/master/docs/build/run-time-library-behavior.md
	
	Dalam proses attach, fungsi _DllMainCRTStartup() akan melakukan beberapa rutinitas berikut:
	- setup buffer security check
	- menginisialisasi CRT dan	pustaka lainnya
	- inisialisasi Run-Type Type Information
	- inisialisasi dan memanggil constructor untuk data static dan non-local
	- inisialisasi Thread-Local Storage
	- menaikkan internal static counter untuk setiap kali attach terjadi
	- dan memanggil DllMain()
	
	Dalam proses detach, fungsi melakukan hal yang serupa dengan urutan terbalik.
	- memanggil Dllmain()
	- menurunkan internal static counter
	- memanggil destructor
	- memanggil fungsi CRT termination dan fungsi atexit()
	- memberi notifikasi kepada semua pustaka yang ada tentang kondisi terminasi yang akan terjadi.

	Ketika attachment counter menjadi nol, fungsi akan mengembalikan nilai FALSE untuk memberitahu OS bahwa DLL
	dapat dengan aman di-unload.
*/
BOOL WINAPI _DllMainCRTStartup(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved)
{
	return 1;
}

#ifdef __cplusplus
}
#endif
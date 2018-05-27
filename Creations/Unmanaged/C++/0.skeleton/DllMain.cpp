/*
Pembuatan DLL standard dengan fungsi DllMain.

Compile:
	[gcc]
		(x64)
		$ g++ -m64 -shared -o unmanaged.dll DllMain.cpp -Wl,--out-implib,libDllMain.a

		(x86)
		$ g++ -m32 -shared -o unmanaged.dll DllMain.cpp -Wl,--out-implib,libDllMain.a
	
	[msvc]
	$ cl /Feunmanaged.dll /LD DllMain.cpp

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
	Semua DLL dapat memiliki opsi untuk menggunakan fungsi entrypoint DllMain(). Fungsi ini dipanggil 
	pada saat inisialisasi (attach) dan terminasi (detach). Hal ini memberikan kesempatan untuk melakukan
	allocation maupun release resource yang dibutuhkan.

	Reference: https://github.com/MicrosoftDocs/cpp-docs/blob/master/docs/build/run-time-library-behavior.md

	Jangan sekali-kali melakukan hal-hal berikut di dalam DllMain:
	- Jangan memanggil fungsi berikut (baik secara langsung maupun tak langsung). Hal ini dapat mengakibatkan
	  deadlock maupun crash:
	  	- LoadLibrary() atau LoadLibraryEx()
		- GetStringTypeA(), GetStringTypeEx(), atau GetStringTypeW()
		- Fungsi-fungsi berkaitan dengan rRegistry. Fungsi registry diimplementasikan di dalam Advapi32.dll 
		  yang mungkin belum dinisialisasi sebelum DLL ini.
		- CreateProcess(), ada kemungkinan memanggil DLL lain.
		- ExitThread(). keluar dari thread ketika DLL detach dapat mengakibatkan memperoleh loader lock sehinga
		  berkemungkinan menghasilkan deadlock atau crash.
	- Synchronize dengan thread lain, untuk mencegah deadlock.
	- Meminta synchronization object yang dimiliki oleh kode yang sedang meminta loader lock. Hal ini untuk
	  menghindari deadlock.
	- Inisialisasi COM thread dengan CoInitializeEx(), dalam kondisi tertentu fungsi ini akan memanggil LoadLibraryEx().
	- Menggunakan fungsi memory-management dari C Run Time (CRT). Jika CRT belum diinisialisasi, pemanggilan terhadap
	  fungsi ini dapat menyebabkan crash.
	= Memanggil fungsi di User32.dll atau Gdi32.dll.
	- Menggunakan managed code.

	Hal-hal berikut terbilang aman untuk dilakukan di dalam DllMain:
	- Inisialisasi member dan struktur data yang statis selama compile time.
	- Membuat dan menginisialisasi synchronization objects.
	- Mengalokasikan memory dan inisialisasi data structure dinamis (hindari fungsi-fungsi yang ada di daftar di atas).
	- Setup Thread Local Storage (TLS).
	- Open, read, dan write sebuah file.
	- Memanggil fungsi di Kernel32.dll (kecuali fungsi yang disebutkan di atas).
	- Mengisi nilai global pointers ke NULL, menunda inisialisasi anggota kelas yang dinamis.
	  Di Windows Vista, kita dapat menggunakan fungsi One-Time Initialization untuk menjamin bahwa block kode dieksekusi
	  hanya sekali dalam lingkungan multithreaded.
*/
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH: 
			// Inisialisasi sekali untuk setiap process baru.
			// Return FALSE untuk menggagalkan DLL load.
            break;
		case DLL_THREAD_ATTACH:  
			// Lakukan inisialisasi yang spesifik terhada thread.
            break;
		case DLL_THREAD_DETACH:  
			// Lakukan clean-up yang spesifik terhadap thread.
            break;
		case DLL_PROCESS_DETACH: 
			// Lakukan pembersihan seluruh resource jika diperlukan.
            break;
	}
}

#ifdef __cplusplus
}
#endif
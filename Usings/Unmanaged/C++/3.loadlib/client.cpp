/*
Memuat pustaka saat runtime.
Menggunakan Win32 API untuk memuat isi pustaka ke memory dan mendapatkan 
alamat fungsi yang dibutuhkan.

Explicit linking

Compile:
	[gcc]
		(x64)
		$ g++ -m64 client.cpp -o client.exe -std=c++11
		
		(x86)
		$ g++ -m32 client.cpp -o client.exe -std=c++11

	(msvc)
	$ cl /nologo client.cpp
	
run:
	$ client.exe
*/

#include <windows.h>
#include <cstdio>

/* 
Proses linking di MSVC membutuhkan hal ini. 
Tapi jika tidak ingin menambahkan ketiga lib tersebut dalam source code, maka perintah kompilasi harus
diubah menjadi:
	$ cl /LD unmanaged.cpp kernel32.lib advapi32.lib user32.lib
*/
#ifdef _MSC_VER
#pragma comment(lib,"advapi32")
#pragma comment(lib,"kernel32")
#pragma comment(lib,"user32")
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
Setiap fungsi memiliki type signature yang memberikan beberapa informasi, seperti:
	- type dari return value;
	- jumlah argument;
	- type dari setiap argument.

Fungsi GetProcAddress() hanya mengembalikan alamat dari suatu fungsi.
Fungsi ini kemudian diperlakukan sebagai sebuah function pointer melalui typecasting.
Dengan demikian, user bertanggung jawab untuk melakukan typecast ke signature yang tepat.
*/

// Opsi 1: Kita dapat membuat sebuah typedef
typedef void (__stdcall* func_world)();

int main()
{
	int result;

	// Memuat modul DLL ================================================
	// Sebelum mendapatkan alamat dari fungsi terlebih dahulu kita harus memuat pustaka
	// ke memory dan memetakannya ke area yang masih tersedia.
	// Kita dapat menyerahkan tanggung jawab ini ke API
	// HMODULE hDll = LoadLibrary("unmanaged.dll");
	auto unmanaged = LoadLibrary("unmanaged.dll");
	if (unmanaged == nullptr) 
	{
		MessageBox(nullptr, "Cannot locate the DLL file", "Error", MB_OK);
		return 1;
	}
	
	// Cari fungsi berdasarkan nama ====================================
	// Opsi 1: buat sebuah type untuk fungsi kemudian typecast 
	//         alamat yang dikembalikan GetProcAddress
	func_world world = (func_world) GetProcAddress(unmanaged, "world");

	// Opsi 2: buat function pointer secara langsung kemudian lakukan
	//         typecast ke alamat yang dikembalikan
	int (*calculate)(int) = (int (*)(int)) GetProcAddress(unmanaged, "calculate");

	// Opsi 3: gunakan auto (hanya berlaku mulai C++11)

	// Periksa apakah ada fungsi yang tak dapat ditemukan
	if (world==nullptr || calculate == nullptr) 
	{
		MessageBox(nullptr, "Could not locate the function", "Error", MB_OK);
		return 2;
	}

	// Panggil fungsi ==================================================
	// Memanggil fungsi static world()
	world();

	// Memanggil calculate()
	result = calculate(0);
	printf("The result of calculation(0) is %d\n", result);

	// Bebaskan area memory jika modul DLL tak terpakai lagi.	
	FreeLibrary(unmanaged);
}

#ifdef __cplusplus
}
#endif
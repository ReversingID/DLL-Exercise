/*
Membuat DLL saat tidak ada file .LIB / .A yang ditemukan.

Implicit linking.

Buat sebuah file .DEF dan gunakan untuk membuat file .LIB

catatan: 
GCC tidak membutuhkan file .DEF dan dapat bekerja dengan 
hanya file DLL saja.

Creating import lib:
	(mingw)
	$ dlltool -l unmanaged.lib -d unmanaged.def

	[msvc]
		(x64)
		$ lib /def:unmanaged.def /machine:x64

		(x86)
		$ lib /def:unmanaged.def /machine:x86

Compile:
	[gcc]
		(x64)
		$ g++ -m64 client.cpp -o client.exe -L. -lunmanaged
		
		(x86)
		$ g++ -m32 client.cpp -o client.exe -L. -lunmanaged

	(msvc)
	$ cl /nologo client.cpp unmanaged.lib
	
Run:
	$ client.exe
*/
#include <windows.h>
#include <cstdio>

#ifdef __cplusplus
extern "C" {
#endif

/*
Deklarasi semua fungsi yang akan diimpor.
Selain mendeklarasikan secara manual, prototipe fungsi dapat pula
dideklarasikan di sebuah file header.
*/
__declspec(dllimport) void world();
__declspec(dllimport) int  calculate(int n);

int main()
{
	int result;

	// Memanggil fungsi static world()
	world();

	// Memanggil calculate()
	result = calculate(0);
	printf("Hasil dari calculate(0) adalah %d\n", result);
	
	return 0;
}


#ifdef __cplusplus
}
#endif
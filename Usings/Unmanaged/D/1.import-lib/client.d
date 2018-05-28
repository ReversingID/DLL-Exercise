/*
Client sederhana untuk menggunakan unmanaged.dll (lihat bagian Creations/Unmanaged)

Implicit linking
DLL memiliki file .LIB yang dapat digunakan untuk membuat daftar definition di IAT.
Tes ini akan menguji skenario dimana file .LIB tersedia.

Compile:
	[dmd]
		(x64)
			$ dmd -m64 client.d unmanaged.lib

		(x86)
			$ dmd -m32 client.d unmanaged.lib

Run:
	$ client.exe
*/

import std.stdio;

// Impor interface (unmanaged.di)
// Beritahu compiler tentang fungsi apa saja yang akan kita impor
import unmanaged;

int main()
{
    int result;

	// Memanggil fungsi static world()
	world();

	// Memanggil calculate()
	result = calculate(0);
	writefln("Hasil dari calculate(0) adalah %d\n", result);
	
	return 0;
}
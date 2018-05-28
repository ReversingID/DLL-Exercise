/*
Memuat pustaka saat runtime.
Menggunakan Win32 API untuk memuat isi pustaka ke memory dan mendapatkan 
alamat fungsi yang dibutuhkan.

Explicit linking

Compile:
	[dmd]
		(x64)
			$ dmd -m64 client.d

		(x86)
			$ dmd -m32 client.d
	
run:
	$ client.exe
*/

import core.runtime;

// Impor fungsi berikut agar kita dapat memuat fungsi dari DLL
extern(Windows) void* LoadLibraryA(const char* filename);
extern(Windows) void* GetProcAddress(void*, const char*); 
extern(Windows) bool  FreeLibrary(void*);

/*
Setiap fungsi memiliki type signature yang memberikan beberapa informasi, seperti:
	- type dari return value;
	- jumlah argument;
	- type dari setiap argument.

Fungsi GetProcAddress() hanya mengembalikan alamat dari suatu fungsi.
Fungsi ini kemudian diperlakukan sebagai sebuah function pointer melalui typecasting.
Dengan demikian, user bertanggung jawab untuk melakukan typecast ke signature yang tepat.
*/

// Buat alias / typedef
alias extern(C) void function()    func_world;
alias extern(C) int  function(int) func_calculate;

int main()
{
    import std.stdio;

    int result;

    // Memuat modul DLL ================================================
	// Sebelum mendapatkan alamat dari fungsi terlebih dahulu kita harus memuat pustaka
	// ke memory dan memetakannya ke area yang masih tersedia.
	// Kita dapat menyerahkan tanggung jawab ini ke API
	auto dll = LoadLibraryA("unmanaged.dll");
	if (dll is null) 
	{
		writeln("Tidak dapat memuat file DLL");
		return 1;
	}
	
	// Cari fungsi berdasarkan nama ====================================
	auto world     = cast(func_world)     GetProcAddress(dll, "world");
    auto calculate = cast(func_calculate) GetProcAddress(dll, "calculate");

	// Periksa apakah ada fungsi yang tak dapat ditemukan
	if ((world is null) || (calculate is null)) 
	{
		writeln("Tidak dapat menemukan fungsi");
		return 2;
	}

	// Panggil fungsi ==================================================
	// Memanggil fungsi static world()
	world();

	// Memanggil calculate()
	result = calculate(0);
	writefln("Hasil dari calculate(0) adalah %d\n", result);
	
	// Bebaskan area memory jika modul DLL tak terpakai lagi.
	FreeLibrary(dll);

    return 0;
}
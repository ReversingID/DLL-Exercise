/*
Pembuatan DLL dengan definition file.

Pembuatan DLL sederhana dengan definition file.

Menggunakan def file untuk mendefinisikan DLL membuat penggunaan perintah `export`
menjadi tidak diperlukan.

Def file atau definition file adalah file yang digunakan untuk mendefinisikan
karakteristik dari objk executable / library yang dihasilkan.

Compile:
	[gcc]
		(x64)
		$ g++ -m64 -shared -o unmanaged.dll unmanaged.cpp

		(x64)
		$ g++ -m32 -shared -o unmanaged.dll unmanaged.cpp

	[msvc]
	$ cl /LD unmanaged.cpp unmanaged.def
	
Run:
	$ rundll32 unmanaged.dll,world

Catatan:
GCC tidak memerlukan DEF file. Namun, MinGW dapat menghasilkan DEF file dari file DLL.
	$ dlltool -z output.def --export-all-symbol unmanaged.dll

atau membuat sebuah interface lib dari DEF file
	$ dlltool -d input.def -l unmanaged.a 
*/

#include <windows.h>

/* 
Proses linking di MSVC membutuhkan hal ini. 
Tapi jika tidak ingin menambahkan ketiga lib tersebut dalam source code, maka perintah kompilasi harus
diubah menjadi:
	$ cl /LD unmanaged.cpp unmanaged.def kernel32.lib advapi32.lib user32.lib
*/
#ifdef _MSC_VER
#pragma comment(lib,"advapi32")
#pragma comment(lib,"kernel32")
#pragma comment(lib,"user32")
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* Ekspor fungsi world() telah dilakukan dengan file DEF */
void world()
{
	DWORD dwtemp = 62;
	TCHAR szname[64], szbuff[MAX_PATH+1];
	
	if (GetUserName(szname, &dwtemp))
		wsprintf(szbuff, "Hello %s, how are you?", szname);
	else
		lstrcpyn(szbuff, "Hello stranger! :D", 18);
	MessageBox(NULL, szbuff, TEXT("Title"), MB_OK | MB_ICONINFORMATION);
}

/* Ekspor fungsi calculate() telah dilakukan dengan file DEF */
int calculate(int n)
{
	return n + 135;
}

// Fungsi _DllMainCRTStartup digunakan untuk membuat DLL minimalis
// Alternatif: menggunakan DllMain()
BOOL WINAPI _DllMainCRTStartup(HINSTANCE hinst, DWORD dwReason, LPVOID lpres)
{
	return 1;
}

#ifdef __cplusplus
}
#endif
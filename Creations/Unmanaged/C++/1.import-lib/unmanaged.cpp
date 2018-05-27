/*
Pembuatan DLL sederhana dengan import lib.

Untuk mengekspor fungsi ke DLL, sebuah macro harus digunakan.
__declspec adalah macro yang spesifik untuk keperluan pembuatan DLL.

Compile:
	[gcc]
		(x64)
		$ g++ -m64 -shared -o unmanaged.dll -Wl,--out-implib,libunmanaged.a unmanaged.cpp

		(x64)
		$ g++ -m32 -shared -o unmanaged.dll -Wl,--out-implib,libunmanaged.a unmanaged.cpp

	[msvc]
	$ cl /LD unmanaged.cpp
	
run
	$ rundll32 unmanaged.dll,world
*/

#include <windows.h>

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


/* Ekspor fungsi world() dengan __declspec(dllexport) */
__declspec(dllexport) 
void world()
{
	DWORD dwtemp = 62;
	TCHAR szname[64], szbuff[MAX_PATH+1];
	
	if (GetUserName(szname, &dwtemp))
		wsprintf(szbuff, "Halo %s, apa kabar?", szname);
	else
		lstrcpyn(szbuff, "Halo saudara! :D", 18);
	MessageBox(NULL, szbuff, TEXT("Title"), MB_OK | MB_ICONINFORMATION);
}

/* Ekspor fungsi calculate() dengan __declspec(dllexport) */
__declspec(dllexport)
int calculate(int n)
{
	return n + 135;
}

// Gunakan _DllMainCRTStartup untuk membuat DLL minimalis
// Alternatif: menggunakan DllMain()
BOOL WINAPI _DllMainCRTStartup(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved)
{
	return 1;
}

#ifdef __cplusplus
}
#endif
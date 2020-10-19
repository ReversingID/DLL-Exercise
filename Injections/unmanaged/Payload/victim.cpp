/*
Ekspor tiga fungsi untuk diimpor oleh `runme`

Compile:
	[gcc]
		(x64)
		$ g++ -m64 -shared -o victim.dll victim.cpp

		(x64)
		$ g++ -m32 -shared -o victim.dll victim.cpp

	[msvc]
	$ cl /LD victim.cpp
*/

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

int __declspec(dllexport) add(int a, int b)
{
	return a + b;
}

int __declspec(dllexport) sub(int a, int b)
{
    return a - b;
}

int __declspec(dllexport) mul(int a, int b)
{
	return a * b;
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
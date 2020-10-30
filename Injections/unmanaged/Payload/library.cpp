/*
	Ekspor fungsi-fungsi yang digunakan oleh `program`.

Compile:
	[msvc]
	$ cl /LD library.cpp

	[clang]
	(x64)
	$ clang++ -m64 -shared -o library.dll library.cpp
	(x86)
	$ clang++ -m32 -shared -o library.dll library.cpp
*/

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

int __declspec(dllexport) add (int a, int b)
{
	return a + b;
}

int __declspec(dllexport) sub (int a, int b)
{
    return a - b;
}

int __declspec(dllexport) mul (int a, int b)
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
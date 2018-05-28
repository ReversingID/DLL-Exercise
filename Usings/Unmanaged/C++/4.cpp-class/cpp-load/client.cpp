/*
Memanggil DLL yang mengekspor fungsi-fungsi sebuah kelas.

Compile:
	[gcc]
		(x64)
		$ g++ -m64 client.cpp -o client.exe -L. -lunmanaged
		
		(x86)
		$ g++ -m32 client.cpp -o client.exe -L. -lunmanaged

	(msvc)
	$ cl /nologo client.cpp unmanaged.lib
	
run:
	$ client.exe
*/
#include <cstdio>
#include "DllClass.hpp"

int main()
{
	DllClass dc;
	
	dc.set("Reversing.ID Community");
	printf("%s\n", dc.get());
	
	return 0;
}

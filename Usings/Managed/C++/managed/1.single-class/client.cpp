/*
    Client sederhana untuk menggunakan managed.dll.
    Lihat juga bagian Creations/Managed.

Memanggil managed.dll yang memiliki fungsi:
    - world (static)
    - calculate

Compile:
    $ cl /clr client.cpp

Run: 
    $ client.exe

Note:
    Ini adalah Managed C++, atau C++ dengan .NET Runtime.
*/

// Impor DLL 
#using <managed.dll>

using namespace System;

// Gunakan namespace dari pustaka (library)
using namespace CodeLibrary;

int main(int argc, char* argv[])
{
    int result;
    DllCode^ code = gcnew DllCode();

    // Memanggil fungsi static world()
    DllCode::world();

    // Memanggil calculate()
    result = code->calculate(0);
    System::Console::WriteLine("Hasil dari calculate(0) adalah {0}", result);
}
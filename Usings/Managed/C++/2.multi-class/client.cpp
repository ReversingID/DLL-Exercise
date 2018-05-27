/*
Client sederhana untuk menggunakan managed.dll (lihat bagian Creations/Managed)

Memanggil managed.dll yang memiliki tiga kelas:
    - AddClass
    - MultiClass
    - FactorialClass

Compile:
    $ cl /clr client.cpp

Run: 
    $ client.exe
*/

// Impor DLL 
#using <managed.dll>

using namespace System;

// Gunakan namespace dari pustaka (library)
using namespace CodeLibrary;

int main(int argc, char* argv[])
{
    // Panggil semua fungsi
    // AddClass.Add()
    System::Console::WriteLine("The result of 100 + 35 is {0}", AddClass::Add(100, 35));
    
    // MultiClass.Multiply()
    System::Console::WriteLine("The result of 27 * 5 is {0}", MultiClass::Multiply(27, 5));
    
    // FactorialClass.Factorial()
    System::Console::WriteLine("The result of 5! is {0}", FactorialClass::Factorial(5));
}
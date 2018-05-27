/*
Client sederhana untuk menggunakan managed.dll (lihat bagian Creations/Managed)

Memanggil managed.dll yang memiliki tiga kelas:
    - AddClass
    - MultiClass
    - FactorialClass

Compile:
    (x64)
    $ csc /platform:x64 client.cs /r:managed.dll
    
    (x86)
    $ csc /platform:x86 client.cs /r:managed.dll

Run: 
    $ client.exe
*/
using System;

// Gunakan namespace dari pustaka (library)
using CodeLibrary;

namespace DllClient
{
    public class Client 
    {
        // main function
        public static void Main(String[] args)
        {
            // Panggil semua fungsi
            // AddClass.Add()
            System.Console.WriteLine("The result of 100 + 35 is {0}", AddClass.Add(100, 35));
            
            // MultiClass.Multiply()
            System.Console.WriteLine("The result of 27 * 5 is {0}", MultiClass.Multiply(27, 5));
            
            // FactorialClass.Factorial()
            System.Console.WriteLine("The result of 5! is {0}", FactorialClass.Factorial(5));
            
        }
    }
}
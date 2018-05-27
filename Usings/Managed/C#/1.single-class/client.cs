/*
Client sederhana untuk menggunakan managed.dll (lihat bagian Creations/Managed)

Memanggil managed.dll yang memiliki fungsi:
    - world (static)
    - calculate

Compile:
    $ csc client.cs /r:managed.dll

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
            int result;
            DllCode code = new DllCode();

            // Memanggil fungsi static world()
            DllCode.world();

            // Memanggil calculate()
            result = code.calculate(0);
            System.Console.WriteLine("Hasil dari calculate(0) adalah {0}", result);
        }
    }
}
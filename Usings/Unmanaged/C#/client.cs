/*
Client sederhana untuk menggunakan unmanaged.dll (lihat bagian Creations/Unmanaged)

Memanggil unmanaged.dll yang memiliki fungsi:
    - world (static)
    - calculate

Menggunakan P/Invoke untuk memuat fungsi dari DLL.
Ada beberapa alternatif, lihat lebih banyak contohnya di repository `CodeInterop-Exercise`

Compile:
    (x64)
    $ csc /platform:x64 client.cs
    
    (x86)
    $ csc /platform:x86 client.cs

Run: 
    $ client.exe
*/
using System;
using System.Runtime.InteropServices;

namespace DllClient
{
    public class Client 
    {
        // Deklarasikan fungsi-fungsi yang akan diimpor
        [DllImport("unmanaged.dll")]
        private static extern void world();

        [DllImport("unmanaged.dll")]
        private static extern int  calculate(int n);

        // Main function
        public static void Main(String[] args)
        {
            int result;

            // Memanggil fungsi static world()
            world();

            // Memanggil calculate()
            result = calculate(0);
            System.Console.WriteLine("Hasil dari calculate(0) adalah {0}", result);
        }
    }
}
/*
Membuat managed DLL dengan C#.

Hanya akan ada satu class di dalam DLL.

Sebuah managed DLL tidak dapat dijalankan dengan rundll32, sehingga trik ini tidak dapat diterapkan.
Dengan kata lain diperlukan sebuah program yang memuat DLL dan memanggil fungsi di dalamnya.

Compile:
    (x64)
    $ csc /platform:x64 /t:library /out:managed.dll managed.cs

    (x86)
    $ csc /platform:x86 /t:library /out:managed.dll managed.cs

Run:
    $ 
*/

using System;
using System.Windows.Forms;

/*
Pertanyaan:
    - Apa yang DLL ekspor?
    - Ketika kode direkonstruksi dengan decompiler .NET, seberapa banyak kemiripannya
      antara source code dan kode hasil dekompilasi?
    - Apa perbedaan antara method static dan non-static?
    - Apakah entry point DLL sama dengan entry point kode .NET?
*/

namespace CodeLibrary
{
    public class DllCode
    {
        // setter dan getter untuk id
        public string id { get; set; }

        // Static method
        public static void world()
        {
            MessageBox.Show("Halo dunia! Pesan ini berasal dari (Managed) DLL");
        }

        // Instance method
        public int calculate(int n)
        {
            return n + 135;
        }
    }
}
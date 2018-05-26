/*
Membuat managed DLL dengan C#.

Akan terdapat tiga class di dalam DLL.
    - AddClass
    - MultiClass
    - FactorialClass (*)

Compile:
    (x64)
    $ csc /platform:x64 /t:library /out:managed.dll AddClass.cs MultiClass.cs FactorialClass.cs
    
    (x86)
    $ csc /platform:x86 /t:library /out:managed.dll AddClass.cs MultiClass.cs FactorialClass.cs
*/

using System;

/*
Pertanyaan:
    - Apa yang DLL ekspor?
    - Ketika kode direkonstruksi dengan decompiler .NET, seberapa banyak kemiripannya
      antara source code dan kode hasil dekompilasi?
    - Ketika hanya kode ini yang di-compile, apa isi dari DLL?
*/

namespace CodeLibrary
{
    public class FactorialClass
    {
        public static int Factorial(int i)
        {
            return ((i <= 1) ? 1 : (i * Factorial(i-1)));
        }
    }
}
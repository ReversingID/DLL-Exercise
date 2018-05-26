/*
Membuat managed DLL dengan C++.

Akan terdapat tiga class di dalam DLL.
    - AddClass
    - MultiClass
    - FactorialClass (*)

Compile:
     $ cl /clr /LD /Femanaged.dll AddClass.cpp MultiClass.cpp FactorialClass.cpp
*/

using namespace System;

/*
Questions:
    - What do the DLL export?
    - What's the difference of managed and unmanaged DLL?
    - When reconstruct in .NET-aware decompiler, how much the similarity
      of the soure code and decompiled code?
    - What's the difference of static and non-static method?
*/

namespace CodeLibrary
{
    public ref class FactorialClass
    {
    public:
        static int Factorial(int i)
        {
            return ((i <= 1) ? 1 : (i * Factorial(i-1)));
        }
    };
};
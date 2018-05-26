' Membuat managed DLL dengan Visual Basic.
'
' Akan terdapat tiga class di dalam DLL.
'     - AddClass
'     - MultiClass
'     - FactorialClass (*)
' 
' Compile:
'     (x64)
'     $ vbc /platform:x64 /t:library /out:managed.dll AddClass.vb MultiClass.vb FactorialClass.vb
'
'     (x86)
'     $ vbc /platform:x86 /t:library /out:managed.dll AddClass.vb MultiClass.vb FactorialClass.vb
'
' Run:
'    $ 

imports System

' Pertanyaan:
'     - Apa yang DLL ekspor?
'     - Ketika kode direkonstruksi dengan decompiler .NET, seberapa banyak kemiripannya
'       antara source code dan kode hasil dekompilasi?
'     - Ketika hanya kode ini yang di-compile, apa isi dari DLL?

namespace CodeLibrary
    public class FactorialClass
        public shared function Factorial(i as integer) as integer
            return if((i <= 1), 1, (i * FactorialClass.Factorial(i-1)))
        end function
    end class
end namespace
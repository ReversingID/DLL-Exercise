' Membuat managed DLL dengan Visual Basic.
'
' Akan terdapat tiga class di dalam DLL.
'     - AddClass (*)
'     - MultiClass
'     - FactorialClass
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
    public class AddClass
        public shared function Add(a as integer, b as integer) as integer
            return a + b
        end function
    end class
end namespace
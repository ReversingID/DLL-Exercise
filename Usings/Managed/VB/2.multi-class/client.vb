' Client sederhana untuk menggunakan managed.dll (lihat bagian Creations/Managed)
'
' Memanggil managed.dll yang memiliki tiga kelas:
'     - AddClass
'     - MultiClass
'     - FactorialClass
'
' Compile:
'    (x64)
'    $ vbc /platform:x64 /r:managed.dll client.vb
'
'    (x86)
'    $ vbc /platform:x86 /r:managed.dll client.vb
'
' Run: 
'     $ client.exe

imports System

' Gunakan namespace dari pustaka (library)
imports CodeLibrary

namespace DllClient
    public class Client 
        ' main function
        public shared sub Main(args as String())
            ' Panggil semua fungsi
            ' AddClass.Add()
            System.Console.WriteLine("The result of 100 + 35 is {0}", AddClass.Add(100, 35))

            ' MultiClass.Multiply()
            System.Console.WriteLine("The result of 27 * 5 is {0}", MultiClass.Multiply(27, 5))

            ' FactorialClass.Factorial()
            System.Console.WriteLine("The result of 5! is {0}", FactorialClass.Factorial(5))
        end sub 'Main
    end class   'Client
end namespace   'DllClient
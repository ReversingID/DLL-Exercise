' Client sederhana untuk menggunakan managed.dll (lihat bagian Creations/Managed)
'
' Memanggil managed.dll yang memiliki fungsi:
'     - world (static)
'     - calculate
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
            dim result as integer
            dim code as DllCode = new DllCode()

            ' Memanggil fungsi static world()
            DllCode.world()

            ' Memanggil calculate()
            result = code.calculate(0)
            System.Console.WriteLine("Hasil dari calculate(0) adalah {0}", result)
        end sub 'Main
    end class   'Client
end namespace   'DllClient
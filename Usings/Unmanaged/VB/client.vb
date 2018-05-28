' Client sederhana untuk menggunakan unmanaged.dll (lihat bagian Creations/Unmanaged)
'
' Memanggil unmanaged.dll yang memiliki fungsi:
'     - world (static)
'     - calculate
'
' Menggunakan P/Invoke untuk memuat fungsi dari DLL.
' Ada beberapa alternatif, lihat lebih banyak contohnya di repository `CodeInterop-Exercise`
'
' Compile:
'     (x64)
'     $ vbc /platform:x64 client.vb
    
'     (x86)
'     $ vbc /platform:x86 client.vb
'
' Run: 
'     $ client.exe

imports System
imports System.Runtime.InteropServices

' P/Invoke
' Memanggil kode native (unmanaged) dari managed code.
' Prototipe dari fungsi bertipe native harus dideklarasikan terlebih dahulu.

' Kebalikan dari P/Invoke adalah Reverse p/Invoke
' Contoh penggunaan kode Reverse P/Invoke dapat dilihat pada repository `CodeInterop-Exercise`

namespace DllClient
    public class Client 

        '===== Native declaration ====================================
        ' Kedua cara deklarasi di bawah ini valid
        <DllImport("unmanaged.dll")>
        private shared  sub world()
        end sub

        public  declare function calculate lib "unmanaged.dll" (n as integer) as integer

        '===== Main functions ========================================
        public shared sub Main(args as String())
            dim result as integer

            ' Memanggil fungsi static world()
            world()

            ' Memanggil calculate()
            result = calculate(0)
            System.Console.WriteLine("Hasil dari calculate(0) adalah {0}", result)
        end sub 'Main
    end class   'Invoke
end namespace   'DllClient
' Membuat managed DLL dengan Visual Basic.
'
' Hanya akan ada satu class di dalam DLL.
'
' Sebuah managed DLL tidak dapat dijalankan dengan rundll32, sehingga trik ini tidak dapat diterapkan.
' Dengan kata lain diperlukan sebuah program yang memuat DLL dan memanggil fungsi di dalamnya.
'
' Compile:
'    (x64)
'    $ vbc /platform:x64 /t:library /out:managed.dll managed.vb
'
'    (x86)
'    $ vbc /platform:x86 /t:library /out:managed.dll managed.vb
'
' Run:
'    $ 

imports System
imports System.Windows.Forms

' Pertanyaan:
'     - Apa yang DLL ekspor?
'     - Ketika kode direkonstruksi dengan decompiler .NET, seberapa banyak kemiripannya
'       antara source code dan kode hasil dekompilasi?
'     - Apa perbedaan antara method static dan non-static?
'     - Apakah entry point DLL sama dengan entry point kode .NET?

namespace CodeLibrary
    public class DllCode
        ' setter dan getter untuk id
        public property id as string

        ' A static method
        public shared sub world()
            MessageBox.Show("Halo dunia! Pesan ini berasal dari (Managed) DLL")
        end sub

        ' Instance method
        public function calculate(n as integer) as integer
            return n + 135
        end function
    end class
end namespace
/*
Membuat managed DLL dengan C++.

Hanya akan ada satu class di dalam DLL.

Sebuah managed DLL tidak dapat dijalankan dengan rundll32, sehingga trik ini tidak dapat diterapkan.
Dengan kata lain diperlukan sebuah program yang memuat DLL dan memanggil fungsi di dalamnya.

Compile:
    $ cl /clr /LD /Femanaged.dll managed.cpp

Run:
    $ 

catatan:
Dibandingkan dengan C#, C++ merupakan bahasa yang lebih low-level. Meskipun dapat dikompilasi untuk
target CLR, terdapat beberapa hal tertentu yang harus dilakukan secara eksplisit. Sebagai contoh, 
sebuah DLL untuk CLR harus di-include secara manual.
*/

// Untuk marshaling dari unmanaged code ke managed code
#include <msclr/marshal_cppstd.h>

// Include .NET DLL
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Windows::Forms;

/*
Pertanyaan:
    - Apa yang DLL ekspor?
    - Apa perbedaan antara managed dan unmanaged DLL?
    - Ketika kode direkonstruksi dengan decompiler .NET, seberapa banyak kemiripannya
      antara source code dan kode hasil dekompilasi?
    - Apa perbedaan antara method static dan non-static?
    - Apakah entry point DLL sama dengan entry point kode .NET?
*/

namespace CodeLibrary
{
    public ref class DllCode
    {
    public:
        // Static method
        static void world()
        {
            // Unmanaged string akan dikonversi menjadi managed string
            // Meskipun managed code dapat pula mengonversi dari char* atau wchar_t*
            // kita lakukan ini secara manual untuk memberikan pemahaman di balik layar.
            std::string stdstr = "Halo dunia! Pesan ini berasal dari (Managed) DLL";
            String ^ message = msclr::interop::marshal_as<System::String^>(stdstr);

            // Hal ini ekivalen dengan perintah berikut:
            //String^ message = "Halo dunia! Pesan ini berasal dari (Managed) DLL";

            MessageBox::Show(message);
        }

        // Instance method
        int calculate(int n)
        {
            return n + 135;
        }
    };
};
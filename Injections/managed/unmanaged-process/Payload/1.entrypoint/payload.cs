/*
    Inject DLL ini ke dalam process.
    Sebuah MessageBox akan tampil dengan sebuah pesan.

Compile:
    (x64)
    $ csc /platform:x64 /t:library /out:payload.dll payload.cs

    (x86)
    $ csc /platform:x86 /t:library /out:payload.dll payload.cs
*/

using System;
using System.Windows.Forms;

namespace RevID
{
    public class Payload
    {
        /*
            entrypoint yang akan dijalankan ketika DLL Injection berhasil.
        */
        public static int entry (string args)
        {
            MessageBox.Show ("Hello from Payload.dll");
            return 0;
        }
    }
}

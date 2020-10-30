/*
    Inject DLL ini ke dalam process.
    Pembacaan dan penulisan wilayah memory tertentu akan terjadi.
    Dalam kasus ini, nilai variabel global akan dimodifikasi.

Compile:
    (x64)
    $ csc /unsafe /platform:x64 /t:library /out:payload.dll payload.cs

    (x86)
    $ csc /unsafe /platform:x86 /t:library /out:payload.dll payload.cs

Note:
    Sesuaikan alamat variabel global sebelum melakukan kompilasi!!
*/

using System;
using System.Runtime.InteropServices;

namespace RevID
{
    public class Payload
    {
        // lokasi variabel global
        // pastkan alamat ini sesuai
        const long ADDR_DIRECT  = 0x7FF63EA73000;
        const long ADDR_MARSHAL = 0x7FF63EA73004;

        public static int entry (string args)
        {
            mtd_direct();
            mtd_marshal();

            return 0;
        }

        /*
        akses baca/tulis langsung ke memory menggunakan pointer.
        gunakan unsafe untuk akses langsung memory.
        */
        public static unsafe void mtd_direct ()
        {
            int * ptr_global = (int*) ADDR_DIRECT;

            Console.WriteLine ("Nilai variabel direct = {0}", *ptr_global);
            *ptr_global = 1337;
            Console.WriteLine ("Nilai variabel direct = {0}", *ptr_global);
        }
        
        /*
        menggunakan Marshal dan IntPtr
        */
        public static void mtd_marshal ()
        {
            IntPtr address = new IntPtr (ADDR_MARSHAL);
            int value;

            // baca nilai
            value = Marshal.ReadInt32 (address);
            Console.WriteLine ("Nilai variabel marshal = {0}", value);

            // tulis nilai
            Marshal.WriteInt32 (address, 1337);

            // baca ulang nilai
            value = Marshal.ReadInt32 (address);
            Console.WriteLine ("Nilai variabel marshal = {0}", value);
        }
    }
}

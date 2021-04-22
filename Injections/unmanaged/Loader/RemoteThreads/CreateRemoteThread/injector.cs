/*
    C# Injector
    Archive of Reversing.ID

    Inject DLL (native) ke process.
    Menggunakan API CreateRemoteThread()

Compile:
    $ csc injector.cs

Run:
    $ injector <PID> <dll path>
*/

using System;
using System.Runtime.InteropServices;
using System.Threading.Tasks;


/*
Ide:
    - Buat sebuah thread baru di remote / target process.
    - Thread baru akan mengeksekusi DLL yang di-inject.

Langkah-langkah:
    - Dapatkan process handle dengan berbekal PID.
    - Alokasikan ruang yang cukup di memory untuk menyimpan string dari DLL
    - Tulis path DLL ke process.
    - Dapatkan address dari modul kernel32.dll dan fungsi LoadLibraryA.
    - Buat thread baru di proses target dengan fungsi LoadLibraryA() sebagai entrypoint dan 
      DLL path sebagai argument.
*/

namespace Injector 
{
    class Program
    {
        /* -- Windows API untuk melakukan injeksi -- */
        [DllImport("kernel32.dll")]
        public static extern 
        IntPtr  OpenProcess (int dwDesiredAccess, int bInheritHandle, int dwProcessId);
 
        [DllImport("kernel32.dll")]
        public static extern 
        int     CloseHandle (IntPtr hObject);
 
        [DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
        public static extern 
        IntPtr  VirtualAllocEx (IntPtr hProcess, IntPtr lpAddress, int dwSize, int flAllocationType, int flProtect);
 
        [DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
        public static extern 
        IntPtr  VirtualFreeEx (IntPtr hProcess, IntPtr lpAddress, int dwSize, int dwFreeType);

        [DllImport("kernel32.dll")]
        public static extern 
        bool    WriteProcessMemory (IntPtr hProcess, IntPtr lpBaseAddress, string lpBuffer, int nSize, out IntPtr lpNumberOfBytesWritten);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        public static extern 
        IntPtr  GetModuleHandle (string lpModuleName);

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi, ExactSpelling = true)]
        public static extern 
        IntPtr  GetProcAddress (IntPtr hModule, string procName);
 
        [DllImport("kernel32")]
        public static extern 
        IntPtr  CreateRemoteThread (IntPtr hProcess, IntPtr lpThreadAttributes, int dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, int dwCreationFlags, out IntPtr lpThreadId);
 
        [DllImport("kernel32.dll")]
        public static extern 
        UInt32  WaitForSingleObject (IntPtr hHandle, UInt32 dwMilliseconds);


        /* -- Konstanta -- */
        // warning: PROCESS_ALL_ACCESS adalah macro, ini adalah nilai di Windows 10
        // privileges
        const int  PROCESS_ALL_ACCESS = (0x000F0000 | 0x00100000 | 0x00000FFF);

        // memory allocation
        const int  MEM_COMMIT  = 0x00001000;
        const int  MEM_RESERVE = 0x00002000;
        const int  MEM_RELEASE = 0x00008000;
        const int  PAGE_READWRITE = 4;

        // wait object
        const uint INFINITE = 0xFFFFFFFF;


        static void Main (string [] args)
        {
            Console.WriteLine("Injector written in C#");

            if (args.Length != 2)
            {
                Console.WriteLine ("[!] Should have 2 arguments!\n");
                Console.WriteLine ("Usage: injector <PID> <dll path>");
                return;
            }

            inject (args[0], args[1]);
        }

        static int inject (string pid, string dll_path)
        {
            IntPtr remote;
            IntPtr remote_thread;
            IntPtr alloc_addr;
            IntPtr num_written;
            IntPtr tid;
            bool   write_result;

            IntPtr kernel_handle_addr;
            IntPtr load_lib;

            Console.WriteLine ("[+] Starting DLL Injector");

            /*
                [1] buka akses ke remote process

                Semua operasi yang akan dilakukan berikutnya terjadi di process target.
                Untuk itu, process target harus dapat dimanipulasi dengan terlebih dahulu mendapatkan
                akses ke process tersebut.
            */
            remote = OpenProcess (PROCESS_ALL_ACCESS, 0, int.Parse(pid));
            if (remote == null)
            {
                Console.WriteLine ("[!] Cannot open the remote process!");
                return 1;
            }


            /* 
                [2] alokasikan ruang di process

                Pada case ini, ruang memory dialokasikan untuk menyimpan path dan nama file DLL.
                Process kemudian dipaksa melakukan loading terhadap DLL dengan memanfaatkan API.
            */
            alloc_addr = VirtualAllocEx (remote, IntPtr.Zero, dll_path.Length + 1, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
            if (alloc_addr == null)
            {
                Console.WriteLine ("[!] Cannot allocate memory on remote process!");
                return 2;
            }
            Console.WriteLine ("    [=] Address allocated: " + alloc_addr);


            /*
                [3] tulis path DLL

                path dan nama file DLL harus dapat dijangkau oleh aplikasi, baik secara absolute maupun
                relative.
            */

            Console.WriteLine ("[+] Writing DLL path to current process space\n");
            write_result = WriteProcessMemory (remote, alloc_addr, dll_path, dll_path.Length, out num_written);
            if (! write_result)
            {
                VirtualFreeEx (remote, alloc_addr, 0, MEM_RELEASE);
                CloseHandle(remote);

                Console.WriteLine ("[!] Cannot allocate memory on remote process!");
                return 3;
            }
            Console.WriteLine ("    [=] Writing success!");


            /*
                [4] Resolve address kernel32.dll & LoadLibraryA

                API LoadLibraryA dibutuhkan untuk memuat DLL.
                Fungsi ini didefinisikan di dalam kernel32.dll dan sebagian besar process memuat
                library tersebut (default).
            */

            Console.WriteLine ("[+] Resolving call specific functions and libraries");
            kernel_handle_addr = GetModuleHandle ("kernel32.dll");
            Console.WriteLine ("    [=] Resolved kernel32 library at " + kernel_handle_addr);

            // API: FARPROC GetProcAddress(HMODULE hModule,LPCSTR lpProcName);
            load_lib = GetProcAddress (kernel_handle_addr, "LoadLibraryA");
            Console.WriteLine ("    [=] Resolved LoadLibraryA function at " + load_lib);


            /*
                [5] Eksekusi LoadLibraryA sebagai thread baru.

                LoadLibraryA() dipanggil sebagai thread baru dengan target path yang telah
                dialokasikan.
            */

            Console.WriteLine ("[+] Creating Remote Thread to load our DLL");
            remote_thread = CreateRemoteThread (remote, IntPtr.Zero, 0, load_lib, alloc_addr, 0, out tid);
            Console.WriteLine ("    [=] Spawning thread with ID: " + tid);
            if (remote_thread == null)
            {
                VirtualFreeEx(remote, alloc_addr, 0, MEM_RELEASE);
                CloseHandle(remote);

                Console.WriteLine ("[!] Cannote create remote thread!");
                return 4;
            }

            WaitForSingleObject(remote_thread, INFINITE);

            VirtualFreeEx(remote, alloc_addr, 0, MEM_RELEASE);
            CloseHandle(remote);

            return 0;
        }
    }
}
# Injections

Beberapa skenario penggunaan `DLL Injection`. Setiap skenario dapat diimplementasikan dalam beberapa bahasa yang berbeda namun untuk penyederhanaan, implementasi bahasa C digunakan sebagai acuan.

## Unmanaged DLL Injection Exercise

Skenario menyuntikkan `Unmanaged DLL` ke dalam proses native (unmanaged code) dan menjalankan kode di dalam DLL.

Secara garis besar, setiap DLL yang dimuat akan menjalankan sebuah `entrypoint`. `Entrypoint` kemudian digunakan untuk menjalankan payload.

## Techniques

Beberapa teknik yang dapat digunakan untuk memaksa sebuah proses untuk memuat DLL:

- Binary Planting

    Dijelaskan pula pada `Binary Planting` di [attack.md](../../attack.md). `Binary Planting` adalah teknik meletakkan file di lokasi tertentu sehingga dapat dimuat dan dieksekusi oleh aplikasi. Beberapa metode yang relevan antara lain:

    * AppInit_DLLs
    * KnownDll 
    * Search Order Hijacking.

- `SetWindowsHookEx()` API

    Menggunakan API `SetWindowsHookEx` untuk memasang sebuah `application-defined hook` di `hook-chain` yang ada. Terdapat beberapa `hook-chains` yang didukung antara lain: `CBT`, `Journal`, `Window Messages`, `Keyboard`, `Mouse`, dll.

    Teknik ini menginstruksikan OS untuk menyuntikkan `custom-hook` berupa DLL ke proses lain. Hal ini dapat bekerja ketika proses mengimpor / menggunakan fungsionalitas dari `user32.dll`.

- `Image File Execution Options (IFEO)` key

    IEFO merupakan mekanisme untuk selalu menjalankan aplikasi tertentu dalam kondisi didebug oleh sebuah debugger. Hal ini berguna untuk menginvestigasi masalah pada startup code dari sebuah aplikasi (terutama sebuah service).

    Untuk menggunakan IFEO, sebuah key dengan nama aplikasi harus diciptakan di `registry`. Sebagai contoh untuk memicu IFEO terhadap aplikasi "Target" maka key `"HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\Target"` harus tersedia dengan sebuah value bernama "Debugger".

- Remote Threads

    Teknik yang umum dilakukan.

    Menciptakan sebuah thread baru di remote process untuk memuat sebuah DLL. Secara sederhana, thread harus melakukan `LoadLibrary()`.

    Beberapa varian API yang tersedia:

    * CreateRemoteThread()
    * NTCreateThread()
    * QueueUserAPC()
    * RtlCreateUserThread()
    * SetThreadContext()

- Implicitly tracking all process

    Teknik ini bertumpu kepada hooking terhadap Process Creations API seperti `CreateProcess()`. Dengan melakukan tracking terhadap penciptaan process, kita memiliki kesempatan untuk menginjeksi DLL ke process baru.
    
    Berikut adalah ide dasar dari teknik tersebut:

    * enumerasi semua proses yang ada.
    * injeksi DLL hook kepada semua proses yang berjalan. Hal ini dilakukan untuk melakukan injeksi terhadap aplikasi yang dijalankan sebelum injector dieksekusi.
    * Hook `k32!CreateProcessInternalW` di setiap proses, atau di `ntdll!NtCreateProcess`.
    * DLL yang diinjeksikan harus secara eksplisit melakukan hook terhadap API dan melakukan hal-hal berikut:

        - Create child process dalam kondisi suspend
        - inject hook
        - resume proses.
            
- AppCompat Shimming Layer

    Teknik ini menggunakan fasilitas `AppCompat` dan `Shimming Database`.

    Terdapat sebuah System DLL bernama `AppPatch.dll` yang akan membaca `Shimming Database (SDB)`. Database ini memiliki format yang custom dan mendeskripsikan berbagai compatibility shimming / patching.
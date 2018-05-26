# Injections

Beberapa contoh kasus yang dapat diselesaikan menggunakan DLL Injection. Setiap skenario ini diimplementasikan dalam beberapa bahasa yang berbeda, namun semua kode mengacu kepada sumber utama yaitu implementasi dalam bahasa C.

# DLL Injection ecercise

DLL Injection bekerja dengan cara menipu / memaksa sebuah proses untuk memuat suatu DLL yang dikehendaki. Kode di dalam DLL akan dieksekusi sebagai bagian dari proses target dan akan dapat melakukan berbagai hal yang dapat dilakukan oleh proses.

Sebagaimana yang telah dijelaskan pada file [attack.md](attack.md), DLL Injection digunakan untuk memengaruhi perilaku sebuah proses secara langsung.

Secara detail, setiap DLL yang dimuat akan menjalan sebuah entry-point. Bagaimana seandainya terdapat kode malicious yang melakukan perubahan terhadap kode ataupun data di proses asli?

# Techniques

Bagian ini akan menjelaskan beberapa teknik yang dapat digunakan untuk memaksa sebuah proses untuk memuat sebuah DLL yang diinginkan. Teknik-teknik tersebut antara lain:

- Binary Planting

    Bagian ini telah dijelaskan pula pada `Binary Planting` di [attack.md](../attack.md)
    Binary Planting adalah teknik untuk meletakkan malicious file di suatu lokasi tertentu sehingga dapat dimuat dan dieksekusi oleh aplikasi.
    Secara spesifik, beberapa metode yang relevan antara lain:

    * AppInit_DLLs
    * KnownDll 
    * Search Order Hijacking.

- SetWindowsHookEx()

    Fungsi API SetWindowsHookEx dapat digunakan untuk menginstal sebuah application-defined hook di hook-chain yang ada. Terdapat beberapa hook-chains yang didukung, antara lain CBT, Journal, Window messages, keyboard, mouse, dll. 
    Secara detail, teknik ini menginstruksikan OS untuk menginjeksi custom-hook berupa DLL ke proses lain yang relevan.
    Hal ini dapat bekerja ketika proses mengimpor / menggunakan fungsionalitas dari user32.dll.

- Image File Execution Options (IFEO) key

    IEFO merupakan mekanisme untuk selalu menjalankan sebuah aplikasi tertentu dalam kondisi didebug oleh sebuah debugger. Hal ini cukup berguna untuk menginvestigasi masalah pada startup code dari sebuah aplikasi (terutama sebuah service).
    Untuk menggunakan IFEO, sebuah key dengan nama aplikasi harus diciptakan dengan di registry. Sebagai contoh untuk memicu IFEO terhadap aplikasi "Target", maka key "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\Target" harus tersedia dengan sebuah value bernama "Debugger".

- Remote Threads

    Menciptakan sebuah thread baru di remote process untuk memuat sebuah DLL. Secara sederhana, thread harus melakukan `LoadLibrary()` harus melakukan beberapa pemanggilan API di konteks aplikasi tersebut.
    Beberapa varian API yang tersedia:

    * CreateRemoteThread()
    * NTCreateThread()
    * QueueUserAPC()

- Implicitly tracking all process

    Teknik ini bertumpu kepada hooking terhadap Process Creations API seperti `CreateProcess()`. Dengan melakukan tracking terhadap penciptaan process, kita memiliki kesemptan untuk menginjeksi DLL ke process baru.
    
    Berikut adalah ide dasar dari teknik tersebut:

    * enumerasi semua proses yang ada.
    * injeksi DLL hook kepada semua proses yang berjalan. Hal ini dilakukan untuk melakukan injeksi terhadap aplikasi yang dijalankan sebelum injector dieksekusi.
    * Hook k32!CreateProcessInternalW di setiap proses, atau di ntdll!NtCreateProcess.
    * DLL yang diinjeksikan harus secara eksplisit melakukan hook terhadap API dan melakukan hal-hal berikut:

        - Create child process dalam kondisi suspend
        - inject hook
        - resume proses.
            
- AppCompat Simming Layer

    Teknik ini menggunakan fasilitas AppCompat dan Shimming Database.

    Terdapat sebuah System DLL bernama AppPatch.dll yang akan membaca shimming database (SDB). Database ini memiliki format yang custom dan mendeskripsikan berbagai compatibility shimming / patching.
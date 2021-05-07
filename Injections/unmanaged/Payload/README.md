DLL Injections Payload by Exercise
----

Ini adalah latihan melakukan injeksi dengan beberapa payload.

#### Setup

* Compile `library.cpp` menjadi `library.dll`
* Compile `program.cpp` menjadi `program.exe`.
* Compile injector (pilih di [Loader](../Loader)).
* Jalankan `program.exe` dan perhatikan PID yang diperlihatkan.
* Untuk setiap latihan, lakukan:

    - Compile DLL payload
    - Inject payload dengan `injector <PID> <DLL path>`

### Exercise 0: (Entrypoint)

Goal: berhasil injeksi DLL dan memunculkan MessageBox

Tools:

- Process Hacker 2

Steps:

- Buka `ProcessHacker2` dan amati daftar module DLL yang telah dimuat `program.exe`
    - apa saja module DLL yang dimuat?
- Lakukan DLL injection dan perhatikan perubahan yang terjadi.
    - pesan apa yang muncul?
    - apakah entrypoint (DllMain / _DllMainCRTStartup) dieksekusi?
- Kembali amati module pada `program.exe`
    - apakah ada `payload.dll`?
- Unload `payload.dll` (menu 5)
    - pesan apa yang muncul?
    - apakah entrypoint (DllMain / _DllMainCRTStartup) dieksekusi?

### Exercise 1: Self-Unload DLL

Goal: unload DLL berdasarkan kondisi tertentu (key ditekan)

Tools:

- Process Hacker 2

Steps:

- Lakukan DLL injection dan buka `ProcessHacker2` untuk mengamati daftar module.
    - apakah `payload.dll` telah dimuat?
- Tekan tombol `CapsLock`
    - apa pesan yang muncul?
    - apakah `payload.dll` masih ada?

### Exercise 2: (Memory Patch)

Goal: baca tulis memory

Steps:

- Periksa alamat dari variabel global di program.exe (menu 4).
- Sesuaikan alamat variabel global di payload.
- Perhatikan ketika DLL injection terjadi, apakah nilai variabel global berubah?

### Exercise 3 (IAT Patch)

Goal: modifikasi alamat fungsi pada IAT (Import Address Table)

Steps:

- Perhatikan alamat dari semua fungsi di modul (menu 2)
- Perhatikan kembali alamat ketika DLL injection telah terjadi.
- Apakah IAT? Dan apakah yang telah kita lakukan terhadapnya?
- Unload `payload.dll` (menu 5) dan perhatikan apakah yang terjadi pada IAT?

### Exercise 4 (Function Trampoline)

Goal: hooking fungsi / mengarahkan pemanggilan fungsi ke implementasi lain.

Steps:

```Under construction```

### Exercise 5 (Argument Sniff)

Goal: memeriksa dan mengubah nilai pada setiap argumen fungsi.

Steps:

```Under Construction```

### Exercise 6 (Return Value Sniff)

Goal: memodifikasi nilai kembalian fungsi

Steps:

```Under Construction```
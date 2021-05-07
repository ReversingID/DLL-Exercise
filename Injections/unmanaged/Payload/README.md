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

### Exercise 0: Entry Point

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

### Exercise 2: Memory Patch

Goal: baca tulis memory

Steps:

- Periksa alamat dari variabel global di `program.exe` (menu 4).
    - sesuaikan alamat variabel global di `payload.c`.
- Lakukan DLL injection
    - apakah nilai variabel global berubah?

### Exercise 3: IAT Patch

Goal: modifikasi alamat fungsi pada IAT (Import Address Table)

Tools:

- Process Hacker 2

Steps:

- Periksa alamat fungsi di `program.exe` (menu 2)
    - apa alamat dari fungsi `add()`, `sub()`, dan `mul()`?
- Lakukan DLL injection
    - apa alamat dari fungsi `add()`, `sub()`, dan `mul()`?
- Unload `payload.dll` (menu 5)
    - apa alamat dari fungsi `add()`, `sub()`, dan `mul()`?

Questions:

- Apakah yang dimaksud dengan IAT (`Import Address Table`)?
- Bagaimana struktur dalam for `PE`?
- Apa yang telah terjadi saat DLL injection?

### Exercise 4: Memory Scanning

Goal: mendapatkan alamat berisi array of byte.

Tools:

- Process Hacker 2

Steps:

- Periksa alamat dan 10 byte awal fungsi `f()` (menu 1)
    - sesuaikan array `signature` di `payload.c`
- Lakukan DLL injection
    - catat semua `offset` beserta `region` yang ditampilkan
- Buka `ProcessHacker` dan amati `modules` dari `program.exe`.
    - cari rentang dimana setiap offset berada
    - tuju offset dan konfirmasi bahwa array berada di offset tersebut.

### Exercise 5: Function Trampoline

Goal: hooking fungsi / mengarahkan pemanggilan fungsi ke implementasi lain.

Steps:

```Under construction```

### Exercise 6: Argument Sniff

Goal: memeriksa dan mengubah nilai pada setiap argumen fungsi.

Steps:

```Under Construction```

### Exercise 7: Return Value Sniff

Goal: memodifikasi nilai kembalian fungsi

Steps:

```Under Construction```
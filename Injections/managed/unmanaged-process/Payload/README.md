DLL Injection Payload (Exercise)
----

Ini adalah latihan melakukan injeksi `Managed DLL` ke `Unmanaged Process` serta beberapa payload yang dapat digunakan.

#### Setup 

* Compile `library.cpp` menjadi `library.dll` dan `program.cpp` menjadi `program.exe`.
* Compile `bootstrap.cpp` menjadi `bootstrap.dll`.
* Compile injector, sembarang injector native dapat digunakan.
* Jalankan `program.exe` dan perhatikan PID yang diperlihatkan.
* Untuk setiap latihan, lakukan:

    - Compile DLL payload
    - Letakkan `payload.dll` di direktori yang sama dengan `bootstrap.dll`
    - Inject payload dengan `injector <PID> <lokasi bootstrap.dll>`

#### Exercise 1 (Entrypoint)

Goal: berhasil injeksi DLL dan memunculkan MessageBox

Steps:

- Perhatikan apa yang terjadi ketika DLL injection terjadi.
- Apakah entrypoint dieksekusi?
- Modifikasi `payload.cs` dan `bootstrap.cpp` untuk menggunakan nama method yang berbeda sebagai entrypoint.

#### Exercise 2 (Memory Patch)

Goal: baca tulis memory dari managed code.

Steps:

- Periksa alamat dari variabel global di `program` (menu 4).
- Sesuaikan alamat variable global di payload.
- Perhatikan ketika DLL injection terjadi, apakah nilai variabel global berubah?

#### Exercise 3 (IAT Patch)

Goal: modifikasi alamat fungsi pada IAT (Import Address Table)

Steps:

- Perhatikan alamat dari semua fungsi di module (menu 2).
- Perhatikan kembali alamat ketika DLL Injection terjadi.
- Apakah IAT? Dan apakah yang telah kita lakukan terhadapnya?
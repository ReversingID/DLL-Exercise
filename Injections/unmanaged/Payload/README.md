DLL Injections Payload by Exercise
----

Ini adalah latihan melakukan injeksi dengan beberapa payload.

#### Setup

* Compile `library.cpp` menjadi `library.dll` dan `program.cpp` menjadi `program.exe`.
* Compile injector (pilih di [Loader](../Loader)).
* Jalankan `program.exe` dan perhatikan PID yang diperlihatkan.
* Untuk setiap latihan, lakukan:

      - Compile DLL payload
      - Inject payload dengan `injector <PID> <DLL path>`

### Exercise 1 (Entrypoint)

Goal: berhasil injeksi DLL dan memunculkan MessageBox

Steps:

- Perhatikan apa yang terjadi ketika DLL injection terjadi.
- Apakah entrypoint (DllMain / _DllMainCRTStartup) dieksekusi?

### Exercise 2 (Memory Patch)

Goal: baca tulis memory

Steps:

- Periksa alamat dari variabel global di program.exe (menu 4).
- Sesuaikan alamat variabel global di payload.
- Perhatikan ketika DLL injection terjadi, apakah nilai variabel global berubah?

### Exercise 3 (IAT Patch)

Goal: modifikasi alamat fungsi pada IAT (Import Address Table)

Steps:

- Perhatikan alamat dari semua fungsi di modul (menu 2)
- Perhatikan kembali alamat ketika DLL injection telah terjadi
- Apakah IAT? Dan apakah yang telah kita lakukan terhadapnya?

### Exercise 4 (Function Trampoline)

Goal: hooking fungsi / mengarahkan pemanggilan fungsi ke implementasi lain.

Steps:

```Under construction```

### Exercise 5 (Argument Sniff)

Goal: memeriksa dan mengubah nilai pada setiap argumen fungsi.

Steps:

```Under Construction```

### Exercise 6

Goal: memodifikasi nilai kembalian fungsi

Steps:

```Under Construction```
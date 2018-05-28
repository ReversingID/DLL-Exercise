DLL Injections Payload by Exercise
----

### Setup dan Persiapan

* Compile victim.cpp (sebagai DLL) dan runme.cpp (sebagai .EXE).
* Compile injector (pilih di [Loader](../Loader)).
* Jalankan runme dan perhatikan PID yang diperlihatkan.
* Untuk setiap latihan, lakukan:

      - Compile DLL payload
      - Inject payload dengan `injector <PID> <DLL path>`


### Exercise 1

- Perhatikan apa yang terjadi ketika DLL injection terjadi.
- Apakah entrypoint (DllMain / _DllMainCRTStartup) dieksekusi?

### Exercise 2

- Periksa alamat dari variabel global di runme (menu 4).
- Sesuaikan alamat variabel global di payload.
- Perhatikan ketika DLL injection terjadi, apakah nilai variabel global berubah?

### Exercise 3

- Perhatikan alamat dari semua fungsi di modul (menu 2)
- Perhatikan kembali alamat ketika DLL injection telah terjadi
- Apakah IAT? Dan apakah yang telah kita lakukan terhadapnya?

### Exercise 4

```Function Trampoline```

### Exercise 5

```Argument Sniff```

### Exercise 6

```Return Value Sniff```

### Exercise 7

```Module Scan & patch```
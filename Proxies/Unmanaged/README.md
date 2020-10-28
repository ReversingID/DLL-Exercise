# Exercise

Pada contoh case ini melibatkan 3 (tiga) source code:

- program
- target
- proxy

`Program` adalah sebuah executable yang menjadi driver. Program akan memanggil dua fungsi dari `target`, yaitu `intercept` dan `forward`. Masing-masing fungsi akan menerima dan memproses argumen. Proxy adalah DLL yang akan mengimplementasikan `intercept` sebelum akhirnya meneruskan pemanggilan ke `target`. Sementara fungsi `forward` akan diteruskan begitu saja ke `Targat`.

## Exercise 1

Menjalankan `program` secara normal dengan memuat DLL `target`.

- Compile target sebagai `target.dll`
- Compile program sebagai `program.exe`
- Jalankan program
- Perhatikan output ketika pemanggilan `intercept` dan `forward` terjadi.

## Exercise 2

Menjalankan `program` dengan memuat DLL `proxy`.

- Compile target sebagai `target.dll`
- Compile program sebagai `program.exe`
- Ubah nama `target.dll` menjadi `original.dll`.
- Compile proxy sebagai `proxy.dll`
- Ubah nama `proxy.dll` menjadi `target.dll`.
- Jalankan program
- Perhatikan output ketika pemanggilan `intercept` dan `forward` terjadi.
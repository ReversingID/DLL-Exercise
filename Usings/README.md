# Usings

Teknik-teknik yang digunakan untuk memuat DLL baik mengimplementasikan `Load-Time Dynamic Linking` maupun `Run-Time Dynamic Linking`.

## Introduction

Untuk mempermudah pengembangan, sebuah aplikasi dapat dipecah menjadi beberapa bagian atau modul. Fungsi-fungsi yang dibutuhkan oleh aplikasi dapat disimpan dalam file eksternal yang disebut library atau pustaka. Ketika aplikasi berjalan, aplikasi akan dapat memanggil fungsi-fungsi yang terdapat di file eksternal selayaknya melakukan pemanggilan fungsi secara lokal.

Terdapat dua macam operasi loading sebuah DLL, yakni:

- Load-time dynamic linking
- Run-time dynamic linking

### Load-Time Dynamic Linking

Aplikasi memiliki sebuah daftar fungsi yang diimpor dari fle eksternal yang bernama `Import Address Table (IAT)`. Ketika aplikasi dimuat pertama kali oleh OS, file pustaka (DLL) yang dibutuhkan akan turut dimuat oleh OS. Pemuatan / loading ini akan dilakukan secara otomatis tanpa campur tangan aplikasi. Ketika load berhasil, setiap fungsi yang ada dalam IAT akan terisi alamat / lokasi sesungguhnya dimana fungsi itu berada.

Untuk menggunakannya, sediakan sebuah file header (.h) dan import library (.lib) ketika melakukan compile dan linking aplikasi.

### Run-Time Dynamic Linking

Aplikasi akan memuat sebuah pustaka secara manual dan mendapatkan alamat dari fungsi yang akan digunakan pada saat aplikasi sedang berjalan (runtime). Untuk mencapai hal ini, pemanggilan terhadap beberapa API dilakukan oleh aplikasi. Dalam loading tipe ini, fungsi-fungsi yang dibutuhkan tidak terdaftar dalam entri IAT.

Unttuk melakukan hal ini, API `LoadLibrary()` atau `LoadLibraryEx()` digunakan untuk melakukan pemuatan pustaka dan pemetaaan file tersebut ke alamat memori tertentu dari aplikasi. Aplikasi kemudian memanggil API lain `GetProcAddress()` untuk mendapatkan alamat dari fungsi DLL yang dibutuhkan. Adapun syarat yang dibutuhkan adalah sama dengan pemuatan saat Load-Time, yaitu fungsi yang akan digunakan haruslah diekspor oleh DLL.
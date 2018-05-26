# Creations

Teknik-teknik yang digunakan untuk membuat DLL dan diimplementasikan dalam beberapa bahasa pemrograman berbeda.

## Introduction

Sebuah DLL merupakan sebuah modul berisi kode dan data yang dapat digunakan oleh modul lain (aplikasi maupun pustaka).

DLL dapat dikelompokkan menjadi dua bagian, yaitu:

- managed DLL
- unmanaged DLL.

Managed dan unmanaged DLL dapat dilihat dari ada atau tidaknya managed code di dalam DLL tersebut. Managed code adalah kode yang diinterpretasikan oleh .NET Framework. Dengan kata lain, managed DLL adalah DLL yang memiliki kode-kode program .NET di dalamnya, baik secara murni maupun bercampur dengan kode-kode native. Sementara unmanaged DLL adalah DLL yang hanya memiliki kode prosesor di dalamnya tanpa ada managed code.

Pembuatan serta pemanfaatan DLL akan sangat berbeda antara managed DLL dan unmanaged DLL. Hal ini dikarenakan perlunya proses ekstra untuk menerjemahkan managed code menjadi sesuatu yang dapat dipanggil oleh unmanaged code.
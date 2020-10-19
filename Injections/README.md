# Injections

Teknik-teknik yang digunakan untuk menyuntikkan (injection) DLL ke dalam sebuah process.

## Introduction

`DLL Injection` adalah mekanisme memaksa process untuk memuat suatu DLL yang dikehendaki. Kode di dalam DLL akan dieksekusi sebagai bagian dari proses target. Tujuan utama dari `DLL Injection` adalah melakukan operasi yang berbeda dengan yang dilakukan oleh process hingga mengubah data atau perilaku dari sebuah process.

DLL dapat dikelompokkan menjadi dua bagian, yaitu:

- managed DLL.
- unmanaged DLL.

`Managed DLL` dan `Unmanaged DLL` DLL dapat dilihat dari ada atau tidaknya managed code di dalam DLL tersebut. Managed code adalah kode yang diterjemahkan oleh layer .NET.

Mekanisme injeksi `Managed DLL` melalui proses awal yang sama dengan `Unmanaged DLL` dengan diakhiri oleh operasi tambahan untuk menginterpretasikan `managed code`.
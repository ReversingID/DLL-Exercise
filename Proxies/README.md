# Proxies

Teknik-teknik yang digunakan untuk membangun `DLL Proxy`.

Teknik-teknik ini dapat diimplementasikan ke beragam bahasa pemrograman, namun bahasa C/C++ akan menjadi implementasi acuan.

## Introduction

`DLL Proxy` merupakan sebuah DLL yang diciptakan sebagai interface untuk menghubungkan (sebagai proxy) antara client dengan komponen lain (DLL). Disebut juga sebagai `DLL wrappers`, `DLL Redirection`. Teknik ini dimungkinkan berkat fitur `Forward Exports`

Teknik `DLL Proxy` digunakan untuk melakukan `Man In the Middle` terhadap fungsi yang diinginkan, mencegat (intercept) pemanggilan fungsi atau meneruskan (forward) ke DLL asli. Beberapa contoh skenario yang memerlukan `DLL Proxy`:

- mencatat atau memodifikasi argumen.
- mencatat atau memodifikasi return value.
- menjalankan kode lain sebelum dan setelah terjadinya pemanggilan fungsi.
- menjalankan kode alternatif.

Tidak semua fungsi di DLL target harus diimplementasikan. Intercept dapat dilakukan terhadap sebagian fungsi sementara sebagian lain akan diteruskan. 

Secara prinsip, aplikasi korban harus memuat DLL Proxy alih-alih DLL target.

## Forward Export

Agar fungsi di DLL dapat dipanggil, fungsi-fungsi tersebut harus disebutkan di dalam `Export Address Table`. Fungsi dikenali berdasarkan nama fungsi (string) atau angka unik sebagai penanda.

Sebuah DLL dapat pula mengekspor fungsi namun tidak mengimplementasikannya. `Forward Export` adalah mekanisme dimana sebuah fungsi yang diekspor akan diimplementasikan di DLL lain.

## DLL Proxy Creation

Daftar fungsi yang diekspor oleh `DLL Proxy` haruslah sama dengan DLL target, baik string maupun ordinal. Proses pembuatan `DLL Proxy` terbagi atas beberapa langkah:

- dapatkan daftar fungsi yang diekspor DLL Target.
- dapatkan prototipe fungsi yang akan dicegat / dimodifikasi.
- implementasikan fungsi di DLL Proxy.
- forward fungsi lain ke DLL Target.

Terdapat beberapa tools untuk mendapatkan daftar fungsi di DLL Target, seperti `dumpbin`.

Mendapatkan prototipe dari fungsi yang diekspor akan menjadi tantangan tersendiri, terutama apabila fungsi tersebut tidak terdokumentasi. Analisis terhadap kode program baik executable maupun DLL target harus dilakukan untuk menentukan argumen dan return value.
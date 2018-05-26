Beberapa serangan yang diketahui terhadap DLL maupun aplikasi yang menggunakan DLL:

- Binary Planting
- DLL Injection
- DLL Proxy

# Binary Planting

Highlight: serangan ini berfokus pada aspek peletakan DLL di lokasi tertentu.

Binary Planting adalah istilah umum untuk serangan yang meletakkan sebuah file berisi kode jahat (malicious code) di direktori lokal maupun remote sehingga aplikasi yang rentan akan memuat dan mengeksekusi DLL tersebut.

Terdapt beberapa istilah yang telah populer sebelumnya, yakni: DLL Hijacking, DLL Preloading, Insecure Library Loading, DLL Load Hijacking, dan DLL Spoofing. Kelima istilah ini mengacu kepada satu makna yang sama yaitu pemuatan eksekusi sebuah pustaka yang malicious. 

Dalam hal ini, istilah Binary Planting merupakan generalisasi dari istilah-istilah tersebut. Pustaka tidak hanya mengacu kepada file dengan ekstensi ".DLL" namun dalam beberapa kasus dapat juga memiliki ekstensi ".OCX", ".NLS", ".TBP", dan beberapa ekstensi lainnya.

Untuk lebih detail, serangan ini dapat diimplementasikan dalam beberapa teknik, yaitu:

### Search Order Hijacking

Ketika mencari DLL yang akan dimuat, OS akan mencari DLL dari lokasi berikut secara berurutan:

- direktori tempat aplikasi dijalankan
- direktori saat ini (current directory)
- direktori sistem, umumnya merupakan C:\Windows\System32
- direktori windows, umumnya merupakan C:\Windows
- direktori yang disebutkan dalam PATH environment variable.

### AppInit_DLLs Value

Terdapat sebuah value di registry yaitu `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows\AppInit_DLLs`. Value ini berisi daftar beberapa DLL. DLL yang ada dalam daftar ini akan dimuat ke seluruh process yang memuat user32.dll selama initial call terhadap DLL tersebut.

- Di Windows Vistal, AppInit_DLLs telah dinonaktifkan secara default.
- Di Windows 7, AppInit_DLLs mendukung signing code
- Di Windows 8, fungsionalitas AppInit_DLLs dinonaktifkan secara keseluruhan ketika Secure Boot diaktifkan.

### KnownDLLs Key

Terdapat sebuah key di registry dengan alamat `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session manager\KnownDLLs`. Key ini berisi value dimana value tersebut merupakan daftar DLL yang akan diperiksa (lookup) ketika loading / pemuatan DLL terjadi. Dengan kata lain, value ini akan dimuat terlebih dahulu apabila value tersebut cocok atau berisi nama DLL dibutuhkan oleh aplikasi.

### DLL Side-Loading Attack

Referensi: [DLL Side-Loading Attack](https://www.fireeye.com/content/dam/fireeye-www/global/en/current-threats/pdfs/rpt-dll-sideloading.pdf)

Serangan ini menggunakan WinSxS assembly untuk memuat DLL tertentu dari daftar SxS.

WinSxS manifest, merupakan sebuah XML yang tertanam di dalam sebuah file executable dan mendeskripsikan dependensi dan pustaka yang dibutuhkan oleh aplikasi. Manifest tersebut memiliki resource dan metadata library. WinSxS didesain untuk memberikan fleksibilitas kepada developer untuk memperbaharui executable secara mudah dengan melakukan replace terhadap binary lama di lokasi yang sama.


# DLL Injection

Highlight: serangan ini berfokus kepada DLL yang dapat melakukan modifikasi tertentu ketika disuntikkan ke dalam aplikasi.

DLL Injection adalah sebuah teknik yang digunakan untuk menjalankan kode dalam address space yang dimiliki oleh process lain dengan cara memaksanya untuk memuat sebuah DLL tertentu. Dalam implementasinya, terdapat sebuah data yang disuntikkan ke dalam aplikasi. Data ini dapat berupa sebuah path menuju sebuah DLL maupun isi sebuah DLL secara keseluruhan.

DLL Injection dapat digunakan untuk memengaruhi perilaku program maupun menjalankan aksi-aksi tertentu yang tidak diantisipasi oleh pembuat aplikasi. Teknik ini banyak digunakan untuk menambahkan fungsionalitas tertentu kepada aplikasi secara temporer. Selain itu teknik ini dapat digunakan untuk melakukan modifikasi atau sekedar pemantauan terhadap aplikasi.

Adapun beberapa hal yang dapat dilakukan antara lain:

- mengetahui dan memodifikasi data yang ada pada alamat tertentu.
- memanggil fungsi yang ada pada alamat tertentu.
- mengubah IAT,
- mengalihkan pemanggilan aplikasi tertentu menuju fungsi yang berbeda. 
- memodifikasi fungsi atau kode yang ada pada alamat tertentu.
- mengetahui kondisi aplikasi saat berjalan, misal argumen yang diberikan ketika memanggil fungsi tertentu.


# DLL Proxy

Highlight: serangan ini berfokus kepada peranan DLL untuk melakukan pemantauan terhadap data.

Secara umum, istilah proxy mengacu kepada suatu pihak yang menerima sesuatu (perintah, pesan, barang, dsb) dan meneruskannya kepada pihak lain. Sebuah proxy sangat dibutuhkan untuk melakukan pembacaan data yang lewat di antara dua buah entitas dan merupakan elemen terpenting dalam serangan MITM (Man In the Middle).

DLL Proxy adalah sebuah DLL yang menjadi proxy antara dua buah entitas yakni aplikasi dan DLL yang akan dipanggil oleh aplikasi tersebut. Pemanggilan fungsi di pustaka akan dilakukan kepada aplikasi proxy seolah-olah pemanggilan terjadi di pustaka yang sesungguhnya. Setelah melakukan prosedur tertentu, pemanggilan akan diteruskan (forward) menuju DLL yang sesungguhnya.

DLL Proxy lazim digunakan untuk melakukan pemantauan terhadap argumen maupun nilai kembalian ketika sebuah fungsi dipanggil.
/*
Memuat pustaka saat runtime.
Menggunakan Win32 API untuk memuat isi pustaka ke memory dan mendapatkan 
alamat fungsi yang dibutuhkan.

Setup Cargo:
    $ cargo new libload

Compile:
    $ cargo build --release

Run: 
    $ libload.exe

Pastikan unmanaged.dll berada di direktori yang sama dengan executables.
*/

extern crate libloading;

use libloading::{Library, Symbol};

/*
Setiap fungsi memiliki type signature yang memberikan beberapa informasi, seperti:
	- type dari return value;
	- jumlah argument;
	- type dari setiap argument.

Fungsi GetProcAddress() hanya mengembalikan alamat dari suatu fungsi.
Fungsi ini kemudian diperlakukan sebagai sebuah function pointer melalui typecasting.
Dengan demikian, user bertanggung jawab untuk melakukan typecast ke signature yang tepat.
*/

// Type alias / typedef
type WorldFunc     = unsafe fn();
type CalculateFunc = unsafe fn(i64) -> i64;

fn main() {
    let unmanaged = Library::new("unmanaged.dll").unwrap();

    unsafe {
        // Dapatkan address dan assign sebagai fungsi
        let world     : Symbol<WorldFunc>     = unmanaged.get(b"world").unwrap();
        let calculate : Symbol<CalculateFunc> = unmanaged.get(b"calculate").unwrap();
        
        // Memanggil fungsi static world()
        world();

        // Memanggil calculate()
        let result : i64 = calculate(0);
        println!("Hasil dari calculate(0) adalah {0}", result);
    }
}
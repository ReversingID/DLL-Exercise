/*
Client sederhana untuk menggunakan unmanaged.dll (lihat bagian Creations/Unmanaged)

Memanggil unmanaged.dll yang memiliki fungsi:
    - world (static)
    - calculate

Compile:
    $ rustc client.rs

Run: 
    $ client.exe
*/

// Impor semua yang ada di "unmanaged.lib"
#[link(name = "unmanaged")]
extern {
    fn world();
    fn calculate(n : i64) -> i64;
}

fn main() {
    unsafe {
        // Memanggil fungsi static world()
        world();

        // Memanggil calculate()
        let result : i64 = calculate(0);
        println!("Hasil dari calculate(0) adalah {0}", result);
    }
}
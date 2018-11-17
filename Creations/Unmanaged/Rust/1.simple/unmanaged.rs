/*
Pembuatan DLL sederhana dengan mengubah tipe executable menjadi lib.

Compile:
    $ rustc --crate-type cdylib unmanaged.rs
	
Run:
	$ rundll32 unmanaged.dll,world
*/
#![crate_type="dylib"]
#![no_mangle]

#[export_name="world"]
pub extern "C" fn world() {
    println!("Halo dari Rust!")
}

#[export_name="calculate"]
pub extern "C" fn calculate(n : i64) -> i64 {
    n + 135
}
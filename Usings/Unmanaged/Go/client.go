/*
Memuat pustaka saat runtime.

Memanggil unmanaged.dll yang memiliki fungsi:
    - world (static)
    - calculate

Compile:
    $ go build client.go

Run: 
    $ client.exe
*/
package main

import (
	"fmt"
	"syscall"
)

/*
Setiap fungsi memiliki type signature yang memberikan beberapa informasi, seperti:
	- type dari return value;
	- jumlah argument;
	- type dari setiap argument.

Go memiliki beberapa fungsi secara built-in untuk memuat dan menggunakan fungsi dari DLL.
*/

// Fungsi yang dipanggil ketika terjadi error
func abort(funcname string, err error) {
	panic(fmt.Sprintf("%s failed: %v", funcname, err))
}

var (
	/*
	Memuat modul DLL
	Sebelum mendapatkan alamat dari fungsi terlebih dahulu kita harus memuat pustaka
	ke memory dan memetakannya ke area yang masih tersedia.
	Kita dapat menyerahkan tanggung jawab ini kepada library "syscall"

	Pastikan DLL dibangun untuk arsitektur yang sama (32-bit/64-bit)
	*/
	unmanaged, _ = syscall.LoadLibrary("unmanaged.dll")

	// Cari fungsi berdasarkan nama
	worldhandle, _     = syscall.GetProcAddress(unmanaged, "world")
	calculatehandle, _ = syscall.GetProcAddress(unmanaged, "calculate")
)

/*
Pemanggilan pustaka dengan Go tidak terlalu intuitif.
Syscall mendefinisikan 5 buah fungsi berbeda untuk memanggil fungsi.
Perbedaan di antara kelimanya adalah dari segi jumlah argumen yang dimiliki dan menjadi
batas banyaknya argumen yang dapat diberikan.

func Syscall(trap, nargs, a1, a2, a3 uintptr) (r1, r2 uintptr, err Errno)
func Syscall6(trap, nargs, a1, a2, a3, a4, a5, a6 uintptr) (r1, r2 uintptr, err Errno)
func Syscall9(trap, nargs, a1, a2, a3, a4, a5, a6, a7, a8, a9 uintptr) (r1, r2 uintptr, err Errno)
func Syscall12(trap, nargs, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12 uintptr) (r1, r2 uintptr, err Errno)
func Syscall15(trap, nargs, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15 uintptr) (r1, r2 uintptr, err Errno)

Kita akan membuat fungsi baru yang melakukan wrapping terhadap pemanggilan 
syscall ke fungsi DLL.

Pemanggilan langsung dapat pula dilakukan.
*/
func world() {
	var nargs uintptr = 0

	// karena nargs bernilai 0 maka tak ada argumen yang di-pass lagi.
	if _, _, err := syscall.Syscall(uintptr(worldhandle), nargs, 0, 0, 0); err != 0 {
		panic(fmt.Sprintf("gagal memanggil %s: %v", "world", err))
	}
}

func calculate(n uintptr) (result int) {
	var nargs uintptr = 1

	// karena hanya membutuhkan satu argumen, maka kita dapat menggunakan syscall 
	// yang dapat menerima maksimum 4 argument
	retval, _, err := syscall.Syscall(uintptr(calculatehandle), nargs, n, 0, 0)
	if err != 0 {
		panic(fmt.Sprintf("gagal memanggil %s: %v\n", calculate, err))
	}
	result = int(retval)
	return
}

func main() {
	defer syscall.FreeLibrary(unmanaged)

	// Memanggil fungsi static world()
    world();

    // Bebaskan area memory jika modul DLL tak terpakai lagi.
	fmt.Printf("Hasil dari calculate(0) adalah %d\n", calculate(0));
}

func init() {
	fmt.Print("Starting up...\n")
}
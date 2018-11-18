/*
Pembuatan DLL sederhana dengan Go secara tak langsung.

Code berikut akan diubah menjadi C archive (.a) dan header (.h).
Keduanya akan dikompilasi bersama dengan file C untuk menghasilkan DLL.

Compile:
	$ go build -buildmode=c-archive unmanaged.go
	$ gcc -shared -pthread -o unmanaged.dll driver.c unmanaged.a -lWinMM -lntdll -lWS2_32

Run:
	$ rundll32 unmanaged.dll,world

Catatan:
	Sample ini akan melakukan interface secara langsung terhadap Win32 API.
	Seluruh konstanta yang dibutuhkan untuk melakukan pemanggilan Win32 API akan didefinisikan.
	Alternatifnya, gunakan package dari pihak ketiga untuk mengakses Win32 API.

*/

// package harus bernama main.
// Go compiler akan membuat package dengan semua dependenci ke dalam satu binary.
package main 


import (
	"C"							// pseudo-package C harus diimport.
	"syscall"
	"unsafe"
)

const (
    MB_OK                = 0x00000000
    MB_OKCANCEL          = 0x00000001
    MB_ABORTRETRYIGNORE  = 0x00000002
    MB_YESNOCANCEL       = 0x00000003
    MB_YESNO             = 0x00000004
    MB_RETRYCANCEL       = 0x00000005
    MB_CANCELTRYCONTINUE = 0x00000006
    MB_ICONHAND          = 0x00000010
    MB_ICONQUESTION      = 0x00000020
    MB_ICONEXCLAMATION   = 0x00000030
    MB_ICONASTERISK      = 0x00000040
    MB_USERICON          = 0x00000080
    MB_ICONWARNING       = MB_ICONEXCLAMATION
    MB_ICONERROR         = MB_ICONHAND
    MB_ICONINFORMATION   = MB_ICONASTERISK
    MB_ICONSTOP          = MB_ICONHAND

    MB_DEFBUTTON1 = 0x00000000
    MB_DEFBUTTON2 = 0x00000100
    MB_DEFBUTTON3 = 0x00000200
    MB_DEFBUTTON4 = 0x00000300
)

func abort(funcname string, err syscall.Errno) {
	panic(funcname + " failed: " + err.Error())
}

var (
	user32, _     = syscall.LoadLibrary("user32.dll")
	messagebox, _ = syscall.GetProcAddress(user32, "MessageBoxW")
)

func StrPtr(s string) uintptr {
	return uintptr(unsafe.Pointer(syscall.StringToUTF16Ptr(s)))
}

func MessageBox(caption, text string, style uintptr) (result int) {
	ret, _, callErr := syscall.Syscall9 (messagebox, 
		4, 0, StrPtr(text), StrPtr(caption), style, 
		0, 0, 0, 0, 0)
	
	if callErr != 0 {
		abort("Call MessageBox", callErr)
	}

	result = int(ret)
	return
}



//export world 
func world() {
	MessageBox("Title", "Halo dari DLL Go", MB_OK | MB_ICONINFORMATION)
}

//export calculate 
func calculate(n int) int {
	return n + 135;
}

// fungsi main() harus dideklarasikan (boleh kosong)
func main() {
	
}
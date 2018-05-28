{
Memuat pustaka saat runtime.
Menggunakan Win32 API untuk memuat isi pustaka ke memory dan mendapatkan 
alamat fungsi yang dibutuhkan.

Explicit linking.

Compile:
    [fpc]
        (x64)
        $ fpc -Px86_64 client.pas
        
        (x86)
        $ fpc -Pi386 client.pas

Run: 
    $ client.exe
}

program client;

{$mode objfpc}{$H+}

uses dynlibs, SysUtils, Windows;

{
Setiap fungsi memiliki type signature yang memberikan beberapa informasi, seperti:
	- type dari return value;
	- jumlah argument;
	- type dari setiap argument.

Fungsi GetProcAddress() hanya mengembalikan alamat dari suatu fungsi.
Fungsi ini kemudian diperlakukan sebagai sebuah function pointer melalui typecasting.
Dengan demikian, user bertanggung jawab untuk melakukan typecast ke signature yang tepat.
}

{ Buat alias / typedef }
type
    TWorld     = procedure;
    TCalculate = function(n : Int64) : Int64;
    
var 
    result    : integer;
    world     : TWorld;
    calculate : TCalculate;
    handle    : TLibHandle;

begin 

    { Memuat modul DLL ============================================================ }
    {
    Sebelum mendapatkan alamat dari fungsi terlebih dahulu kita harus memuat pustaka
    ke memory dan memetakannya ke area yang masih tersedia.
    Kita dapat menyerahkan tanggung jawab ini ke API
    }
    handle := LoadLibrary('unmanaged.dll');
    if handle = NILHandle then 
    begin 
        MessageBox(0,PChar('Tidak dapat memuat file DLL'), PChar('Error'), mb_ok);
        exit;
    end;

    { Cari fungsi berdasarkan nama ================================================ }
    Pointer(world)     := GetProcAddress(handle, 'world');
    Pointer(calculate) := GetProcAddress(handle, 'calculate');

    { Periksa apakah ada fungsi yang tak dapat ditemukan }
    if (@world = nil) or (@calculate = nil) then 
    begin 
		MessageBox(NULL, PChar('Tidak dapat menemukan fungsi'), PChar('Error'), mb_ok);
		exit;
    end;

    { Panggil fungsi ============================================================= }
    { Memanggil fungsi static world() }
    world();

    { Memanggil calculate() }
    result := calculate(0);
    writeln('Hasil dari calculate(0) adalah ' + IntToStr(result));

    { Bebaskan area memory jika modul DLL tak terpakai lagi. }
    UnLoadLibrary(handle);
end.
{
Client sederhana untuk menggunakan unmanaged.dll (lihat bagian Creations/Unmanaged)

Memanggil unmanaged.dll yang memiliki fungsi:
    - world (static)
    - calculate

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

uses SysUtils;

{
Kita harus menyamakan calling convention serta type signature yang digunakan 
dalam pustaka DLL.

Dalam hal ini, diasumsikan bahwa calling conventio yang dipakai adalah stdcall
}

function  calculate(n : Int64) : Int64; stdcall; external 'unmanaged.dll' name 'calculate';

procedure world; external 'unmanaged.dll' name 'world';

var 
    result : integer;

begin 
    { Memanggil fungsi static world() }
    world();

    { Memanggil calculate() }
    result := calculate(0);
    writeln('Hasil dari calculate(0) adalah ' + IntToStr(result));
end.
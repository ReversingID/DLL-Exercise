{
Pembuatan DLL sederhana dengan Pascal.

Compile:
    [fpc]
        (x64)
        $ fpc -Px86_64 unmanaged.pas

        (x86)
        $ fpc -Pi386 unmanaged.pas

run:
    rundll32 hello.dll,world
}

library unmanaged;

{$mode objfpc}{$H+}

uses
    Classes,
    SysUtils,
    Windows;

// Definisikan fungsi calculate() 
function calculate(n : Int64) : Int64; stdcall;
begin 
    result := n + 135;
end;

// Definisikan prosedur world()
procedure world; export;
begin
    MessageBox(0,PChar('This is a message'), PChar('Title'), mb_ok)
end;

// Ekspor fungsi calculate() dan prosedur world()
exports calculate, world;

begin
end.



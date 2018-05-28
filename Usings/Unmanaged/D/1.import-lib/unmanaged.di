/*
Deklarasi interface dari fungsi yang diekspor DLL

Memanggil unmanaged.dll yang memiliki fungsi:
    - world (static)
    - calculate

Compile:
    lihat instruksi di file `client.d`
*/

extern(C) export void world();
extern(C) export int  calculate(int n);
/*
Sebagai korban untuk berbagai latihan dalam DLL injection.

Compile:
	[gcc]
		(x64)
		$ g++ -m64 runme.cpp -o runme.exe -L. -lvictim
		
		(x86)
		$ g++ -m32 runme.cpp -o runme.exe -L. -lvictim

	(msvc)
	$ cl /nologo runme.cpp victim.lib

Run:
    $ runme

*/

#include <stdio.h>
#include <string.h>
#include <process.h>
#include <windows.h>

//=== Forward Declarations ===========================================
int f(int n);

//=== Import Functions ===============================================
int __declspec(dllimport) add(int a, int b);
int __declspec(dllimport) sub(int a, int b);
int __declspec(dllimport) mul(int a, int b);

// ketiga fungsi yang diimpor memiliki type-signature yang sama
// sehingga hanya diperlukan satu alias saja.
typedef int (__stdcall* func_ptr)(int, int);

// menyimpan alamat dari ketiga fungsi.
func_ptr pointers[3];

//=== Global Variables ===============================================
int global = 135;

//=== M A I N ========================================================
int main()
{
    int i = 0;
    int choice = 0;
    int x, y;

    HMODULE  handle;

    // Memuat modul DLL ================================================
    handle = GetModuleHandleA("victim.dll");
    pointers[0] = (func_ptr) GetProcAddress(handle, "add");
    pointers[1] = (func_ptr) GetProcAddress(handle, "sub");
    pointers[2] = (func_ptr) GetProcAddress(handle, "mul");

    printf("My PID is %d\n\n", _getpid());

    do
    {
        printf("Menu ----\n");
        printf("1. Call f() 10 times\n");
        printf("2. List modules\n");
        printf("3. Call add,sub,mul (imported from victim.dll)\n");
        printf("4. Show value of global\n");
        printf("0. Exit\n");
        printf("Your choice? (1/2/3/4): ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 0:
                printf("Exiting...");
                break;
            case 1:
                // Challenge: 
                printf("f() ada di ( 0x%p )\n", f);
                for (i = 0; i < 10; i++)
                {
                    printf("%d ", i);
                    f(i);
                    Sleep(1000);
                }
                break;
            case 2:
                // Challenge #3: Ubah semua 
                printf("add ( 0x%p ) == pointers[0] ( 0x%p )\n", add, pointers[0]);
                printf("sub ( 0x%p ) == pointers[1] ( 0x%p )\n", sub, pointers[1]);
                printf("mul ( 0x%p ) == pointers[2] ( 0x%p )\n", mul, pointers[2]);
                break;
            case 3:
                x = 15;
                y = 5;
                printf("operation on X=15 and Y=5\n");
                printf("add(X,Y) = %d\n", add(x,y));
                printf("sub(X,Y) = %d\n", sub(x,y));
                printf("mul(X,Y) = %d\n", mul(x,y));
                break;
            case 4:
                // Challenge #2: ubah nilai dari variabel ini
                printf("(0x%p) global = %d\n", &global, global);
                break;
            default:
                printf("No such choice!\n");
                break;
        }

        printf("\n\n");
    } while (choice != 0);
}

int f(int n)
{
    printf("Got number: %d\n", n);
}
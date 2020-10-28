/*
    Program yang menjalankan dua fungsi dari DLL target

Compile:
    (msvc)
    $ cl /nologo program.cpp target.lib

Run:
    $ program
*/

#include <stdio.h>
#include <string.h>
#include <windows.h>


//=== Import Functions ===============================================

#ifdef __cplusplus
extern "C" {
#endif 

int __declspec(dllimport) intercept (const char * message);
int __declspec(dllimport) forward (const char * message);

#ifdef __cplusplus
}
#endif 

//=== M A I N ========================================================

int main ()
{
    int choice;
    const char * msg_intercept = "Sending message for intercept()";
    const char * msg_forward   = "Sending message for forward()";

    do 
    {
        printf ("Menu ----\n");
        printf ("1. Call intercept()\n");
        printf ("2. Call forward()\n");
        printf ("0. Exit\n");
        printf ("Your choice? (1/2): ");
        scanf ("%d", &choice);

        switch (choice)
        {
            case 0:
                printf ("Exiting ...");
                break;
            case 1:
                intercept(msg_intercept);
                break;
            case 2:
                forward(msg_forward);
                break;
            default:
                printf ("No such choice!\n");
                break;
        }
        printf ("\n\n");
    } while (choice != 0);

    return 0;
}
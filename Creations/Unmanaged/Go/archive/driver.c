#include <windows.h>
#include "unmanaged.h"

// Pemanggilan fungsi berikut akan memaksa compiler (GCC) untuk 
// link fungsi-fungsi yang dideklarasikan di Go
int main() 
{ 
    world();
    calculate(1);
}
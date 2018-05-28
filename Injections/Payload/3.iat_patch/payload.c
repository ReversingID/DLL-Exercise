/*
Inject DLL ini ke dalam proses.
Mencari fungsi add, sub, dan mul di tabel IAT proses untuk kemudian
dimodifikasi (patch)

Compile:
	[gcc]
		(x64)
		$ gcc -m64 -shared payload.c -o payload.dll
		
		(x86)
		$ gcc -m32 -shared payload.c -o payload.dll

	(msvc)
	$ cl /nologo /LD payload.c

Inject:
    $ injector <PID> payload.dll
*/
#include <windows.h>
#include <stdio.h>


//========= Type Definitions ===========================================
typedef struct 
{
    LPBYTE addr_original;
    LPBYTE addr_hooking;
} IAT_ENTRY;

//========= Global Variables ===========================================
HMODULE  module;
SIZE_T   imp_size;
PIMAGE_IMPORT_DESCRIPTOR imp_desc;

IAT_ENTRY * iat_add;
IAT_ENTRY * iat_sub;
IAT_ENTRY * iat_mul;

//========= Helper Functions ===========================================

void patch_init();
IAT_ENTRY *  patch_it(const char* target_func, LPVOID newfunction);
void patch_restore(IAT_ENTRY * iat_entry);

//========= Export Functions ===========================================
int __declspec(dllexport) add(int a, int b)
{
    printf("\nNot real ADD!\n");
	return a + b + 2;
}

int __declspec(dllexport) sub(int a, int b)
{
    printf("\nNot real SUB!\n");
    return a - b + 2;
}

int __declspec(dllexport) mul(int a, int b)
{
    printf("\nNot real MUL!\n");
	return a * b + 2;
}

//========= DLL Main ===================================================
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID lpres)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            patch_init();
            iat_add = patch_it("add", (LPVOID)add);  // add
            iat_sub = patch_it("sub", (LPVOID)sub);  // sub
            iat_mul = patch_it("mul", (LPVOID)mul);  // mul
            break;
        case DLL_PROCESS_DETACH:
            patch_restore(iat_add);
            patch_restore(iat_sub);
            patch_restore(iat_mul);
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
	return 1;
}

void patch_init()
{
    // Mencari lokasi dari module EXE (program, bukan DLL dari program)
    module = GetModuleHandleA(NULL);

    // Dapatkan DOS header
    PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER) module;

    // Dapatkan NT header dari DOS header
    PIMAGE_NT_HEADERS nt_headers = (PIMAGE_NT_HEADERS)((LPBYTE)dos_header + dos_header->e_lfanew);

    // Dapatkan import descriptor dari NT header
    imp_desc = (PIMAGE_IMPORT_DESCRIPTOR)((LPBYTE)dos_header + nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    // Informasi ukuran IAT juga diberikan oleh NT headers
    imp_size = (SIZE_T) ((LPBYTE)dos_header + nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size);
}

IAT_ENTRY *  patch_it(const char* target_func, LPVOID newfunction)
{
    IAT_ENTRY * retval = NULL;

    // Setiap file PE memiliki IMAGE_IMPORT_DESCRIPTOR masing-masing
    // Cari fungsi yang tepat dan kemudian di-patch!
    
    // Iterasi untuk setiap IMAGE_IMPORT_DESCRIPTOR (setiap DLL)
    for (IMAGE_IMPORT_DESCRIPTOR * iid = imp_desc; iid->Name != NULL; iid++) 
    {
        // untuk setiap DLL, iterasi fungsi
        for (int func_idx = 0; *(func_idx + (LPVOID*)(iid->FirstThunk + (SIZE_T)module)) != NULL; func_idx++) 
        {
            // Dapatkan field name
            char * name = (char*)(*(func_idx + (SIZE_T*)(iid->OriginalFirstThunk + (SIZE_T)module)) + (SIZE_T)module + 2);
            
            // Bandingkan, jika kedua nama sama maka modifikasi fungsi tersebut
            if (!_stricmp(target_func, name))
            {
                // Alokasi struct IAT_ENTRY baru
                retval = (IAT_ENTRY*)malloc(sizeof(IAT_ENTRY));
                void** func_ptr = (func_idx + (LPVOID*)(iid->FirstThunk + (SIZE_T)module));
                
                DWORD oldrights, newrights = PAGE_READWRITE;
                
                // Dapatkan page permission dan set dengan PAGE_READWRITE agar dapat melakukan penulisan.
                VirtualProtect(func_ptr, sizeof(LPVOID), newrights, &oldrights);
                retval->addr_original = *func_ptr;
                *func_ptr = newfunction;                // Tulis entry IAT dengan fungsi baru.
                retval->addr_hooking = newfunction;
                VirtualProtect(func_ptr, sizeof(LPVOID), oldrights, &newrights);

                break;
            }
        }
    }

    return retval;    
}

void patch_restore(IAT_ENTRY * iat_entry)
{
    free(patch_it(iat_entry->addr_hooking, iat_entry->addr_original));
}
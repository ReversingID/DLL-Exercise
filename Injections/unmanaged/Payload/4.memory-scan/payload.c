/*
    Memory Scanning
    Archive of Reversing.ID

    Inject DLL ini ke dalam process.
    Mencari dan menampilkan alamat yang berisi array of byte tertentu.

Compile:
    (msvc)
    $ cl /nologo /LD payload.c

    (clang)
    $ clang -shared payload.c -o payload.dll

    (gcc)
    $ gcc -shared payload.c -o payload.dll

Inject:
    $ injector <PID> payload.dll
*/

#include <windows.h>

#ifdef _MSC_VER
#pragma comment(lib,"advapi32")
#pragma comment(lib,"kernel32")
#pragma comment(lib,"user32")
#endif

// pattern, array of byte yang dicari (fungsi f())
char signature[] = { 0x89, 0x4c, 0x24, 0x08, 0x48, 0x83, 0xec, 0x28, 0x8b, 0x54 };
int  signlen = sizeof(signature) / sizeof(signature[0]);

void computelps (char * pattern, int M, int * lps);
void kmpsearch (char * pattern, DWORD M, char * region, DWORD N);
void memscan (char * pattern, DWORD size, ULONG_PTR start, ULONG_PTR end);


BOOL WINAPI DllMain (HINSTANCE hinst, DWORD dwReason, LPVOID lpres)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            MessageBoxA(NULL, TEXT("Memory Scanning"), TEXT("Payload"), MB_OK);
            memscan (signature, signlen, 0, 0);
            break;
        case DLL_PROCESS_DETACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }
	return 1;
}

/*
    Menghitung LPS (Longest Prefix Suffix) Array
    LPS sebagai preprocessing sebelum pencarian dengan KMP.
*/
void computelps (char * pattern, int M, int * lps)
{
    int len = 0;
    int i = 1;

    // kalkulasi lps[i] untuk i = 1 hingga M-1
    lps[0] = 0;
    while (i < M) 
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else 
        {
            if (len != 0)
                len = lps[len - 1];
            else 
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

/*
    Melakukan pencarian dengan algoritma KMP (Knuth-Morris-Pratt) pada suatu region.
    Menampilkan semua offset ke layar.
    Sebagai alternatif, offset dapat ditampung ke dalam array.
*/
void kmpsearch (char * pattern, DWORD M, char * region, DWORD N)
{
    int   * lps;
    DWORD   i, j;

    lps = malloc(M * sizeof(int));
    computelps (pattern, M, lps);

    i = j = 0;
    while (i < N)
    {
        if (pattern[j] == region[i])
        {
            j++;
            i++;
        }

        // offset ditemukan
        if (j == M)
        {
            printf ("    [+] Pattern found at offset %lu\n", i - j);
            j = lps[j - 1];
        }
        else if (pattern[j] != region[i])
        {
            if (j != 0)
                j = lps[j - 1];
            else 
                i++;
        }
    }

    free (lps);
}

/*
    Melakukan pemindaian di memory.
    Pemindaian dilakukan pada rentang tertentu.
*/
void memscan (char * pattern, DWORD size, ULONG_PTR start, ULONG_PTR end)
{
    SYSTEM_INFO si;
    ULONG_PTR   iter;
    DWORD M, N;
    DWORD i, j;
    
    MEMORY_BASIC_INFORMATION mbi;
    DWORD protflag = (PAGE_GUARD | PAGE_NOCACHE | PAGE_NOACCESS);

    GetSystemInfo(&si);
    if (start == 0)
        start = (ULONG_PTR)(si.lpMinimumApplicationAddress);
    if (end   == 0)
        end   = (ULONG_PTR)(si.lpMaximumApplicationAddress);

    for (iter = start; iter < end - size; )
    {
        // periksa jika page tidak dapat dibaca
        if (VirtualQuery ((LPCVOID)iter, &mbi, sizeof(mbi)))
        {
            // bad region, pencarian tidak dapat dilakukan karena terhalang permission
            if (mbi.Protect & protflag || !(mbi.State & MEM_COMMIT))
            { }
            else 
            {
                printf ("[!] Region Base Address: %p | size: %lu\n", mbi.BaseAddress, mbi.RegionSize);
                kmpsearch (pattern, size, (char *) iter, mbi.RegionSize);
            }
        }

        // maju ke region berikutnya
        iter += mbi.RegionSize;
    }
}
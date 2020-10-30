/*
    Bootstrap
    DLL pertama yang akan diinjeksi ke unmanaged process.
    Mempersiapkan environment untuk dapat menjalankan kode .NET

Compile:
    (msvc)
    $ cl /LD /EHsc bootstrap.cpp user32.lib

Note:
    - payload (managed dll) berada terpisah sebagai payload.dll.
    - payload.dll diasumsikan berada di direktori yang sama
*/

#include <windows.h>
#include <mscoree.h>
#include <metahost.h>
#include <wchar.h>
#include <fstream>

#pragma comment(lib, "mscoree.lib")

EXTERN_C IMAGE_DOS_HEADER __ImageBase;


/* -- hardcoded information -- */
static const LPCWSTR Assembly   = L"payload.dll";
static const LPCWSTR Class      = L"RevID.Payload";
static const LPCWSTR Method     = L"entry";
static const LPCWSTR Param      = L"";
static const LPCWSTR RtVersion  = L"v4.0.30319";

std::wstring GetPayloadDllPath ();

/*
    Mempersiapkan lingkungan untuk menjalankan kode .NET
    Fungsi ini akan dijalankan pada thread terpisah.
*/
DWORD WINAPI CreateDotNetRuntime (LPVOID param)
{
    ICLRRuntimeHost * rthost = NULL;
    ICLRRuntimeInfo * rtinfo = NULL;
    ICLRMetaHost    * mthost = NULL;
    HRESULT result;
    DWORD   retcode = 0;
    BOOL    loadable;

    /*
    memberikan salah satu dari instance: ICLRMetaHost, ICLRMetaHostPolicy, ICLRDebugging.
    pada case ini, kita memerlukan CLRMetaHost
    */
    result = CLRCreateInstance (CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*) &mthost);
    if (FAILED (result))
    {
        MessageBox (NULL, "Unable to Create Instance!", "ERROR", MB_OK);
        return 1;
    }
    
    /*
    Dapatkan ICLRRuntimeInfo untuk versi CLR tertentu.
    Hingga 30/10/2020 terdapat beberapa versi CLR yaitu:
        - v1.0.3705
        - v1.1.4322
        - v2.0.50727
        - v4.0.30319
    
    Memberikan NULL sebagai versi akan memilih versi tertinggi yang dimiliki
    */
    result = mthost->GetRuntime (RtVersion, IID_PPV_ARGS(&rtinfo));
    if (FAILED (result))
    {
        mthost->Release();

        MessageBox (NULL, "Unable to Get Runtime!", "ERROR", MB_OK);
        return 2;
    }

    /*
    memeriksa apakah runtime yang berkaitan dengan interface ini
    dapat dimuat di process.
    */
    result = rtinfo->IsLoadable (&loadable);
    if (FAILED (result) || !loadable)
    {
        rtinfo->Release();
        mthost->Release();

        MessageBox (NULL, "Is not loadable!", "ERROR", MB_OK);
        return 3;
    }

    /*
    memuat CLR ke process.
    */
    result = rtinfo->GetInterface (CLSID_CLRRuntimeHost, IID_PPV_ARGS(&rthost));
    if (FAILED (result))
    {
        rtinfo->Release();
        mthost->Release();

        MessageBox (NULL, "Unable to Get Interface!", "ERROR", MB_OK);
        return 4;
    }

    /*
    jalankan .NET Runtime
    */
    result = rthost->Start();
    if (FAILED (result))
    {
        rthost->Release();
        rtinfo->Release();
        mthost->Release();

        MessageBox (NULL, "Unable to Start!", "ERROR", MB_OK);
        return 5;
    }

    /*
    memuat Managed DLL sebagai Assembly di Application Domain default.
    mulai eksekusi dari Class dan Method yang diberikan.
    */
    result = rthost->ExecuteInDefaultAppDomain (GetPayloadDllPath().c_str(), Class, Method, Param, &retcode);
    if (FAILED (result))
    {
        rthost->Stop();
        rthost->Release();
        rtinfo->Release();
        mthost->Release();

        MessageBox (NULL, "Unable to Execute!", "ERROR", MB_OK);
        return 6;
    }
    
    return 0;
}

/*
    Mendapatkan absolute path dari payload.dll dengan asumsi payload.dll
    berada pada direktori yang sama dengan bootstrap.
*/
std::wstring GetPayloadDllPath ()
{
    std::wstring result;
    int index;

    LPWSTR path = new WCHAR[_MAX_PATH];
    GetModuleFileNameW ((HINSTANCE)&__ImageBase, path, _MAX_PATH);

    result = path;
    index = result.rfind ('\\');
    result.erase (index + 1, result.length() - index - 1);
    result += Assembly;

    return result;
}

/*
    Buat thread baru ketika bootstrap DLL dimuat oleh process
    sehingga persiapan .NET Runtime dilakukan di thread terpisah.
*/
DWORD APIENTRY DllMain (HMODULE mod, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            CreateThread (NULL, NULL, CreateDotNetRuntime, NULL, NULL, NULL);
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
        default:
            break;
    }

    return 1;
}
# DLL Exercises
Simple self-exercises for learning DLL creation and utilization.

### Disclaimer by Reversing.ID

Repository ini digunakan untuk menghimpun informasi dan pengetahuan tentang pembuatan dan pemanfaatan DLL dalam berbagai skenario, terutama untuk keperluan reversing. Repository ini terbuka untuk publik, dapat diakses oleh siapa saja baik oleh internal komunitas Reversing.ID maupun di luar komunitas.

### Content

Repository ini berisi beberapa pokok bahasan yang dibagi menjadi beberapa direktori berbeda.

- Creations
- Injections
- Modifications
- Proxies
- Usings

Masing-masing topik akan membahas teknik-teknik yang relevan dan diimplementasikan menggunakan beberapa bahasa pemrograman yang berbeda.

### General Reference

Knowledge
---

- MSDN Dynamic-Link Libraries - https://msdn.microsoft.com/en-us/library/windows/desktop/ms682589%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396

API
---

- FreeLibrary function https://msdn.microsoft.com/en-us/library/windows/desktop/ms683152%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
- LoadLibrary function https://msdn.microsoft.com/en-us/library/windows/desktop/ms684175(v=vs.85).aspx
- GetProcAddress function https://msdn.microsoft.com/en-us/library/windows/desktop/ms683212(v=vs.85).aspx
- Using Run-Time Dynamic Linking https://msdn.microsoft.com/en-us/library/windows/desktop/ms686944(v=vs.85).aspx

Tools
---

- Dependency Walker http://www.dependencywalker.com/
- dumpbin.exe https://support.microsoft.com/en-us/kb/177429
- rundll32.exe   (RUNDLL32.EXE <dllname>,<entrypoint> <optional arguments>) https://support.microsoft.com/en-us/kb/164787

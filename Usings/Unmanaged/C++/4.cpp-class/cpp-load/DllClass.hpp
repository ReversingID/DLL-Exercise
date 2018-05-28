/*
Deklarasi interface dari kelas yang diekspor DLL

Compile:
    lihat instruksi di file `client.cpp`
*/

class __declspec(dllimport) DllClass
{
private:
	int  m_size;
	char m_internal[64];
public:
	DllClass();
	
	void  set(const char* message);
	char* get();
};
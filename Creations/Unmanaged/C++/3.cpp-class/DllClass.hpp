/**
Deklarasi type dari kelas.
Kelas ini akan diekspor ke DLL.
*/

#ifdef  __EXPORT_CLASS_
#define DECLARE __declspec(dllexport)
#else
#define DECLARE __declspec(dllimport)
#endif

class DECLARE DllClass
{
private:
	int  m_size;
	char m_internal[64];
public:
	DllClass();
	
	void  set(const char* message);
	char* get();
};
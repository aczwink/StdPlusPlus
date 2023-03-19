//Class Header
#include "../../headers/CLibrary.h"
//Global
#include <Windows.h>
//Local
#include "../../headers/Containers/Strings/UTF-16/CUTF16String.h"
//Namespaces
using namespace ACStdLib;

//Constructor
CLibrary::CLibrary(const CPath &refPath)
{
	CString path;
	CUTF16String pathUTF16;
	
	path = refPath.GetString() + ".dll";
	pathUTF16 = path.GetUTF16();
	this->pOSHandle = LoadLibraryW((LPCWSTR)pathUTF16.GetC_Str());
}

//Destructor
CLibrary::~CLibrary()
{
	FreeLibrary((HMODULE)this->pOSHandle);
}

//Public methods
void *CLibrary::GetFunctionAddress(const C8BitString &refName)
{
	return GetProcAddress((HMODULE)this->pOSHandle, refName.GetC_Str());
}
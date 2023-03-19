//Class Header
#include "../../../headers/Filesystem/CPath.h"
//Global
#include <Windows.h>
//Local
#include "../../../headers/Containers/Strings/UTF-16/CUTF16String.h"
#include "../Time/TimeConstants.h"
//Namespaces
using namespace ACStdLib;
//Definitions
#undef CreateDirectory

//Public methods
bool CPath::CreateDirectory()
{
	CUTF16String pathStringUTF16(this->pathString.GetUTF16());

	return CreateDirectoryW((LPCWSTR)pathStringUTF16.GetC_Str(), NULL) != 0;
}

CTime CPath::GetLastModifiedTime() const
{
	int64 timeStamp;
	HANDLE hFile;
	FILETIME modifiedTime;

	CUTF16String fileNameUTF16(this->pathString.GetUTF16());

	hFile = CreateFileW((LPCWSTR)fileNameUTF16.GetC_Str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileTime(hFile, NULL, NULL, &modifiedTime);
	CloseHandle(hFile);

	timeStamp = FileTimeToMilliseconds(modifiedTime);

	return CTime(timeStamp);
}

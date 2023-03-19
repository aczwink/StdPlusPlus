//Class header
#include "../../../../headers/UI/CommonDialogs.h"
//Global
#include <ShlObj.h>
//Local
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Local functions
static int CALLBACK BrowseCallbackProcedure(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
		break;
	case BFFM_SELCHANGED:
		{
			bool enable;
			CFunction<bool(const CPath &)> *pCallback;
			WCHAR buffer[8192];
			CPath path;
			
			pCallback = (CFunction<bool(const CPath &)> *)lpData;
			SHGetPathFromIDListW((LPCITEMIDLIST)lParam, buffer);
			path = CUTF16String((uint16 *)buffer);

			enable = (*pCallback)(path);

			SendMessage(hWnd, BFFM_ENABLEOK, 0, enable);
		}
		break;
	}
	
	return 0;
}

//Functions
bool CommonDialogs::SelectExistingDirectory(const CString &refTitle, const CWindow &refWindow, const CFunction<bool(const CPath &)> &refCallback, CPath &refSelectedPath)
{
	LPITEMIDLIST folderId;
	WCHAR buffer[8192];
	BROWSEINFOW bi;
	CUTF16String title;

	title = refTitle.GetUTF16();
	
	bi.hwndOwner = GET_HWND(&refWindow);
	bi.iImage = 0;
	bi.lParam = (LPARAM)&refCallback;
	bi.lpfn = BrowseCallbackProcedure;
	bi.lpszTitle = (LPCWSTR)title.GetC_Str();
	bi.pidlRoot = nullptr;
	bi.pszDisplayName = buffer;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NONEWFOLDERBUTTON;

	folderId = SHBrowseForFolderW(&bi);

	if(folderId)
	{
		SHGetPathFromIDListW(folderId, buffer);
		refSelectedPath = CUTF16String((uint16 *)buffer);
		
		return true;
	}

	return false;
}
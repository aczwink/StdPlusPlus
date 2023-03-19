//Corresponding header
#include "../../../../headers/UI/CommonDialogs.h"
//Global
#include <ShlObj.h>
//Local
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Namespace functions
CPath CommonDialogs::OpenFile(const CWindow &refParentWnd, const CString &refTitle, const CLinkedList<CTuple<CString, CString>> &refFilters)
{
	uint32 i;
	HRESULT hr;
	IFileOpenDialog *pFD;
	IShellItem *pSI;
	PWSTR pFilePath;
	CPath result;
	CArray<COMDLG_FILTERSPEC> filterSpecs;
	
	//create dialog
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFD));
	ASSERT(SUCCEEDED(hr));

	//set file types
	filterSpecs.Resize(refFilters.GetNumberOfElements());
	for(i = 0; i < refFilters.GetNumberOfElements(); i++)
	{
		filterSpecs[i].pszName = (LPWSTR)refFilters[i].Get<0>().GetUTF16().GetC_Str();
		filterSpecs[i].pszSpec = (LPWSTR)refFilters[i].Get<1>().GetUTF16().GetC_Str();
	}
	pFD->SetFileTypes(refFilters.GetNumberOfElements(), &filterSpecs[0]);
	
	//run dialog
	hr = pFD->Show(GET_HWND(&refParentWnd));
	ASSERT(SUCCEEDED(hr));

	//get result
	hr = pFD->GetResult(&pSI);
	ASSERT(SUCCEEDED(hr));

	hr = pSI->GetDisplayName(SIGDN_FILESYSPATH, &pFilePath);
	ASSERT(SUCCEEDED(hr));

	result = CUTF16String((uint16 *)pFilePath);
	CoTaskMemFree(pFilePath);
	pSI->Release();
	
	pFD->Release();
	
	return result;
}
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
CPath CommonDialogs::SaveFile(const CWindow *pParentWnd, const CString &refTitle, const CLinkedList<CTuple<CString, CString>> &refFilters)
{
	uint32 i;
	HWND hWnd;
	HRESULT hr;
	PWSTR pFilePath;
	IShellItem *pSI;
	IFileSaveDialog *pFD;
	CPath result;
	CArray<COMDLG_FILTERSPEC> filterSpecs;

	hWnd = NULL;
	if(pParentWnd)
		hWnd = GET_HWND(pParentWnd);

	//create dialog
	hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFD));
	ASSERT(SUCCEEDED(hr));

	//set file types
	filterSpecs.Resize(refFilters.GetNumberOfElements());
	for(i = 0; i < refFilters.GetNumberOfElements(); i++)
	{
		filterSpecs[i].pszName = (LPWSTR)refFilters[i].Get<0>().GetUTF16().GetC_Str();
		filterSpecs[i].pszSpec = (LPWSTR)refFilters[i].Get<1>().GetUTF16().GetC_Str();
	}
	hr = pFD->SetFileTypes(refFilters.GetNumberOfElements(), &filterSpecs[0]);
	ASSERT(SUCCEEDED(hr));

	//run dialog
	hr = pFD->Show(hWnd);
	ASSERT(SUCCEEDED(hr));

	//get result
	hr = pFD->GetResult(&pSI);
	ASSERT(SUCCEEDED(hr));

	hr = pSI->GetDisplayName(SIGDN_FILESYSPATH, &pFilePath);
	ASSERT(SUCCEEDED(hr));

	result = CUTF16String((uint16 *)pFilePath);
	
	//clean up
	CoTaskMemFree(pFilePath);
	pSI->Release();
	pFD->Release();

	return result;
}
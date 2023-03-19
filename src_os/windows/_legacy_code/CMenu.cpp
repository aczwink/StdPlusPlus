//Class header
#include "../../../../headers/UI/Menu/CMenu.h"
//Global
#include <Windows.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Constructor
CMenu::CMenu(const CString &refText) : AMenuEntry(refText)
{
	this->pOSHandle = CreateMenu();
}

//Destructor
CMenu::~CMenu()
{
	for(AMenuEntry *const& refpMenuEntry : this->menuEntries)
		delete refpMenuEntry;
	
	DestroyMenu((HMENU)this->pOSHandle);
}

//Public methods
void CMenu::AppendItem(CMenuItem *pItem)
{
	uint32 index;
	MENUITEMINFOW itemInfo;
	CUTF16String textUTF16;

	textUTF16 = pItem->GetText().GetUTF16();
	AppendMenuW((HMENU)this->pOSHandle, MF_STRING, (UINT_PTR)pItem, (LPCWSTR)textUTF16.GetC_Str());
	index = this->menuEntries.GetNumberOfElements();

	//set item info
	itemInfo.cbSize = sizeof(itemInfo);
	itemInfo.fMask = MIIM_DATA;
	itemInfo.dwItemData = (ULONG_PTR)pItem;
	SetMenuItemInfoW((HMENU)this->pOSHandle, index, TRUE, &itemInfo);
	
	this->menuEntries.Push(pItem);
}

void CMenu::AppendSeperator()
{
	AppendMenuA((HMENU)this->pOSHandle, MF_SEPARATOR, 0, nullptr);
}

void CMenu::AppendSubMenu(CMenu *pSubMenu)
{
	CUTF16String textUTF16;
	
	textUTF16 = pSubMenu->GetText().GetUTF16();
	AppendMenuW((HMENU)this->pOSHandle, MF_POPUP | MF_STRING, (UINT_PTR)pSubMenu->pOSHandle, (LPCWSTR)textUTF16.GetC_Str());

	this->menuEntries.Push(pSubMenu);
}
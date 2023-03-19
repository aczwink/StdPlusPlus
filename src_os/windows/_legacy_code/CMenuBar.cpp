//Class header
#include "../../../../headers/UI/Menu/CMenuBar.h"
//Global
#include <Windows.h>
//Local
#include "../../../../headers/UI/Menu/CMenu.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Definitions
#undef AppendMenu

//Local functions
static void UpdateMenuBar(HMENU hMenu)
{
	MENUINFO mi;

	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_MENUDATA;

	GetMenuInfo(hMenu, &mi);

	if(mi.dwMenuData)
		DrawMenuBar((HWND)mi.dwMenuData);
}

//Constructor
CMenuBar::CMenuBar()
{
	MENUINFO menuInfo;
	
	this->pOSHandle = CreateMenu();

	//set menu info
	menuInfo.cbSize = sizeof(menuInfo);
	menuInfo.fMask = MIM_STYLE;
	GetMenuInfo((HMENU)this->pOSHandle, &menuInfo);
	menuInfo.dwStyle |= MNS_NOTIFYBYPOS;
	SetMenuInfo((HMENU)this->pOSHandle, &menuInfo);
}

//Destructor
CMenuBar::~CMenuBar()
{
	DestroyMenu((HMENU)this->pOSHandle);

	for(CMenu *const& refpMenu : this->attachedMenus)
	{
		delete refpMenu;
	}
}

//Public methods
void CMenuBar::AppendMenu(CMenu *pMenu)
{
	CUTF16String textUTF16;
	
	textUTF16 = pMenu->GetText().GetUTF16();
	AppendMenuW((HMENU)this->pOSHandle, MF_POPUP | MF_STRING, (UINT_PTR)pMenu->pOSHandle, (LPCWSTR)textUTF16.GetC_Str());

	this->attachedMenus.Push(pMenu);

	UpdateMenuBar((HMENU)this->pOSHandle);
}
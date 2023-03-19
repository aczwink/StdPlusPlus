//Class Header
#include "CMenu.h"
//Local
#include "CPopupMenu.h"

//Public methods
/*void CMenu::AddSubMenu(IPopupMenu *pPopupMenu)
{
	CPopupMenu *pPopupMenuImpl;
	
	pPopupMenuImpl = (CPopupMenu *)pPopupMenu;
	
	AppendMenuW(this->hMenu, MF_STRING | MF_POPUP, (UINT_PTR)pPopupMenuImpl->hMenu, (LPCWSTR)pPopupMenuImpl->name.GetC_Str());
	
	this->children.Insert(pPopupMenuImpl->GetId(), pPopupMenuImpl);
}

void CMenu::ClearHandles()
{
	this->hMenu = nullptr;
	
	for(const auto &refKV : this->children)
		refKV.value->ClearHandles();
}

void CMenu::DispatchEvent(uint8 menuIdx, uint8 itemIdx)
{
	this->children[menuIdx]->DispatchEvent(itemIdx);
}*/
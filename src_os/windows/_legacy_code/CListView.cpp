//Class header
#include "../../../../headers/UI/Views/CListView.h"
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include "../../../../headers/UI/AWidgetContainer.h"
#include "../../../../headers/UI/Controllers/AListController.h"
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//https://msdn.microsoft.com/en-us/library/windows/desktop/bb775146(v=vs.85).aspx

//Constructor
CListView::CListView(AWidgetContainer *pParent) : AWidget(pParent)
{
	this->pOSHandle = CreateWindowExA(0, WC_LISTBOXA, nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 0, 0, GET_HWND(pParent->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr((HWND)this->pOSHandle, GWLP_USERDATA, (LONG_PTR)this);

	SendMessage((HWND)this->pOSHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}

//Eventhandlers
void CListView::OnModelChanged()
{
	SendMessage((HWND)this->pOSHandle, LB_RESETCONTENT, 0, 0);
	
	if(this->pController)
	{
		uint32 nItems, i;
		CUTF16String text;
		
		nItems = this->pController->GetNumberOfItems();
		SendMessage((HWND)this->pOSHandle, LB_INITSTORAGE, nItems, 50);
		for(i = 0; i < nItems; i++)
		{
			text = this->pController->GetText(i).GetUTF16();
			SendMessageW((HWND)this->pOSHandle, LB_ADDSTRING, 0, (LPARAM)text.GetC_Str());
			SendMessageW((HWND)this->pOSHandle, LB_SETITEMDATA, i, i);
		}
	}
}

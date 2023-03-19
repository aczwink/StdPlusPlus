//Class header
#include "../../../../headers/UI/Views/CCheckListView.h"
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include "../../../../headers/UI/AWidgetContainer.h"
#include "../../../../headers/UI/Controllers/ACheckListController.h"
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//https://msdn.microsoft.com/en-us/library/windows/desktop/bb774737(v=vs.85).aspx

//Constructor
CCheckListView::CCheckListView(AWidgetContainer *pParent) : AWidget(pParent)
{
	DWORD dwStyle, dwCtrlStyle;
	LVCOLUMNW col;

	dwStyle = LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER;
	dwCtrlStyle = LVS_EX_CHECKBOXES | LVS_EX_LABELTIP | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER;
	
	this->pOSHandle = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEWA, nullptr, WS_CHILD | WS_VISIBLE | dwStyle, 0, 0, 0, 0, GET_HWND(pParent->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr((HWND)this->pOSHandle, GWLP_USERDATA, (LONG_PTR)this);

	SendMessage((HWND)this->pOSHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
	ListView_SetExtendedListViewStyleEx((HWND)this->pOSHandle, dwCtrlStyle, dwCtrlStyle);
	
	this->sizingPolicy.SetHorizontalPolicy(CSizingPolicy::EPolicy::Expanding);
	this->sizingPolicy.SetVerticalPolicy(CSizingPolicy::EPolicy::Expanding);

	this->pController = nullptr;
	
	//add columns
	col.fmt = LVCFMT_LEFT;
	col.mask = LVCF_FMT;
	col.pszText = nullptr;
	
	SendMessageW((HWND)this->pOSHandle, LVM_INSERTCOLUMNW, 0, (LPARAM)&col);
	SendMessageW((HWND)this->pOSHandle, LVM_INSERTCOLUMNW, 1, (LPARAM)&col);
}

//Eventhandlers
void CCheckListView::OnModelChanged()
{
	ListView_DeleteAllItems((HWND)this->pOSHandle);
	
	if(this->pController)
	{
		uint32 nItems, i, index;
		LVITEMW item;
		CUTF16String text;
		
		nItems = this->pController->GetNumberOfItems();
		for(i = 0; i < nItems; i++)
		{
			text = this->pController->GetText(i).GetUTF16();

			//add row
			item.mask = LVIF_PARAM;
			item.iItem = i;
			item.iSubItem = 0;
			item.lParam = i;
			
			index = SendMessageW((HWND)this->pOSHandle, LVM_INSERTITEMW, 0, (LPARAM)&item);

			//set col1 to item text
			item.mask = LVIF_TEXT;
			item.iSubItem = 1;
			item.pszText = (LPWSTR)text.GetC_Str();
			item.cchTextMax = text.GetLength() + 1;

			SendMessageW((HWND)this->pOSHandle, LVM_SETITEMW, 0, (LPARAM)&item);
		}
	}
}

void CCheckListView::OnResized()
{
	//LVS_EX_AUTOSIZECOLUMNS didnt work
	
	ListView_SetColumnWidth((HWND)this->pOSHandle, 0, LVSCW_AUTOSIZE);
	ListView_SetColumnWidth((HWND)this->pOSHandle, 1, LVSCW_AUTOSIZE_USEHEADER);
}
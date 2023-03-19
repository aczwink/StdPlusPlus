//Class header
#include "../../../../headers/UI/Containers/CTabBox.h"
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include "../../../../headers/Containers/Strings/UTF-16/CUTF16String.h"
#include "../../../../headers/UI/AWidgetContainer.h"
#include "../../../../headers/UI/Layouts/CGridLayout.h"
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//https://msdn.microsoft.com/en-us/library/windows/desktop/bb760548(v=vs.85).aspx

//Constructor
CTabBox::CTabBox(AWidgetContainer *pParent) : AWidgetContainer(pParent)
{
	this->pOSHandle = CreateWindowExA(0, WC_TABCONTROLA, nullptr, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, GET_HWND(pParent->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr((HWND)this->pOSHandle, GWLP_USERDATA, (LONG_PTR)this);

	SendMessage((HWND)this->pOSHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
	
	this->sizingPolicy.SetHorizontalPolicy(CSizingPolicy::EPolicy::Expanding);
	this->sizingPolicy.SetVerticalPolicy(CSizingPolicy::EPolicy::Expanding);
}

//Public methods
CRect CTabBox::GetChildrenRect() const
{
	CRect rc;

	rc = AWidgetContainer::GetChildrenRect();

	//TODO
	
	return rc;
}
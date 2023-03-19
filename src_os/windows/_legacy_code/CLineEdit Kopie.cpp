//Class header
#include "../../../../headers/UI/Controls/CLineEdit.h"
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include "../../../../headers/UI/AWidgetContainer.h"
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Constructor
CLineEdit::CLineEdit(AWidgetContainer *pParent) : AWidget(pParent)
{
	this->pOSHandle = CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDITA, nullptr, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, GET_HWND(pParent->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr((HWND)this->pOSHandle, GWLP_USERDATA, (LONG_PTR)this);

	SendMessage((HWND)this->pOSHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
	
	this->sizingPolicy.SetHorizontalPolicy(CSizingPolicy::EPolicy::Expanding);
	this->sizingPolicy.SetVerticalPolicy(CSizingPolicy::EPolicy::Fixed);
}

//Public methods
CSize CLineEdit::GetSizeHint() const
{
	//TODO: min width
	//TODO: this seems to be working... dont known how it is with different fonts

	return CSize(0, 21);
}
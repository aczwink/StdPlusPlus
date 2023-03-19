//Class Header
#include "../../../../../headers/GUI/WindowClasses/Controls/CStatusBar.h"
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include "../../../../../headers/Containers/Strings/UTF-16/CUTF16String.h"
#include "../CFullAccessWindow.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::GUI;

//Constructor
/*CStatusBar::CStatusBar(AWindowContainer &refParentWnd) : AChildWindow(refParentWnd)
{
	this->nativeHandle = (uint64)CreateWindowExW(0, STATUSCLASSNAMEW, nullptr, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, GET_HWND(&refParentWnd), nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr((HWND)this->nativeHandle, GWLP_USERDATA, (LONG_PTR)this);
}

//Public methods
void CStatusBar::AutoSize()
{
	SendMessageW((HWND)this->nativeHandle, WM_SIZE, 0, 0);
}

void CStatusBar::SetText(const CUTF8String &refText)
{
	CUTF16String textUTF16(refText);
	
	SendMessageW((HWND)this->nativeHandle, SB_SETTEXT, 0, (LPARAM)refText.GetC_Str());
}*/
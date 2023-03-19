//Class header
#include "../../../../headers/UI/Controls/CProgressBar.h"
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include "../../../../headers/Containers/Strings/UTF-16/CUTF16String.h"
#include "../../../../headers/UI/AWidgetContainer.h"
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

/*
WinAPI Documentation:
https://msdn.microsoft.com/en-us/library/windows/desktop/bb760818(v=vs.85).aspx
*/

//Constructor
CProgressBar::CProgressBar(AWidgetContainer *pParent) : AWidget(pParent)
{
	this->sizingPolicy.SetHorizontalPolicy(CSizingPolicy::EPolicy::Expanding);
	this->sizingPolicy.SetVerticalPolicy(CSizingPolicy::EPolicy::Fixed);
	
	if(WANT_OS_HANDLED(pParent))
	{
		this->pOSHandle = CreateWindowExA(0, PROGRESS_CLASSA, nullptr, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, GET_HWND(pParent->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
		SetWindowLongPtr((HWND)this->pOSHandle, GWLP_USERDATA, (LONG_PTR)this);

		SendMessage((HWND)this->pOSHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
	}
}

//Public methods
CSize CProgressBar::GetSizeHint() const
{
	switch(this->GetRenderMode())
	{
	case ERenderMode::OS:
		{
			SIZE size;

			//TODO: calc min width
			//TODO: this seems to be working... dont known how it is with different fonts

			size.cx = 50;
			size.cy = 25;

			return CSize((uint16)size.cx, (uint16)size.cy);
		}
		break;
	case ERenderMode::Text:
		{
			return CSize(6, 1); //[100%] is 6 chars long, and one line is sufficient
		}
		break;
	}

	NOT_IMPLEMENTED_ERROR;
}

void CProgressBar::SetProgress(float64 value)
{
	this->progress = CLAMP(value, 0, 1);

	SendMessageA((HWND)this->pOSHandle, PBM_SETPOS, (int32)(this->progress * 100), 0);
}
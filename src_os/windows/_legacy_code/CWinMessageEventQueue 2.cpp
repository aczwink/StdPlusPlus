/*
//Class Header
#include "CWinMessageEventQueue.h"
//Local
#include "../../../headers/Containers/CFiniteSet/CFiniteSet.h"
#include "../../../headers/UI/Mouse.h"
#include "../../../headers/GUI/WindowClasses/AWindow.h"
#include "../../../headers/GUI/WindowClasses/CFrame.h"
#include "CMenu.h"
#include "WindowClasses/CFullAccessWindow.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::GUI;
using namespace ACStdLib::GUI::Events;

static EKeyCode MapVirtualKeyToKeyCode(uint32 virtualKey)
{
	switch(virtualKey)
	{
	case VK_LSHIFT:
		return EKeyCode::SHIFT_LEFT;
	case VK_RSHIFT:
		return EKeyCode::SHIFT_RIGHT;
	case VK_SPACE:
		return EKeyCode::SPACE;
	case 0x41:
		return EKeyCode::A;
	case 0x42:
		return EKeyCode::B;
	case 0x43:
		return EKeyCode::C;
	case 0x44:
		return EKeyCode::D;
	case 0x45:
		return EKeyCode::E;
	case 0x46:
		return EKeyCode::F;
	case 0x47:
		return EKeyCode::G;
	case 0x48:
		return EKeyCode::H;
	case 0x49:
		return EKeyCode::I;
	case 0x4A:
		return EKeyCode::J;
	case 0x4B:
		return EKeyCode::K;
	case 0x4C:
		return EKeyCode::L;
	case 0x4D:
		return EKeyCode::M;
	case 0x4E:
		return EKeyCode::N;
	case 0x4F:
		return EKeyCode::O;
	case 0x50:
		return EKeyCode::P;
	case 0x51:
		return EKeyCode::Q;
	case 0x52:
		return EKeyCode::R;
	case 0x53:
		return EKeyCode::S;
	case 0x54:
		return EKeyCode::T;
	case 0x55:
		return EKeyCode::U;
	case 0x56:
		return EKeyCode::V;
	case 0x57:
		return EKeyCode::W;
	case 0x58:
		return EKeyCode::X;
	case 0x59:
		return EKeyCode::Y;
	case 0x5A:
		return EKeyCode::Z;
	}
	
	ASSERTMSG(0, "unmapped key");
}

/*
bool CWinMessageEventQueue::DispatchEvent(AWindow &refWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CFrame &refFrame = (CFrame &)refPopupWnd;
	
	switch(message)
	{
	case WM_ACTIVATE:
		{
			if(wParam == WA_INACTIVE)
				APopupWindow::SetActiveWindow(nullptr);
			else
				APopupWindow::SetActiveWindow(&refPopupWnd);

			g_messageResult = 0;
		}
		break;
	case WM_SETFOCUS:
		{
			AWindow::SetFocusedWindow(&refWnd);
			g_messageResult = 0;
		}
		break;
		/*case WM_COMMAND:
		{
			if(lParam)
			{
				//control
				return false;
			}
			else
			{
				if(HIGH16(wParam))
				{
					//Accelerator
					return false;
				}
				else
				{
					uint8 menuIdx, itemIdx;
					IMenu *pMenu;
					CMenu *pMenuImpl;
					
					//Menu
					
					pMenu = ((CFrame &)refWnd).GetMenu();
					pMenuImpl = (CMenu *)pMenu;

					menuIdx = HIGH8(LOW16(wParam));
					itemIdx = LOW8(wParam);
					
					pMenuImpl->DispatchEvent(menuIdx, itemIdx);
				}
			}
			
			g_messageResult = 0;
		}
		break;*/
	/*
	default:
		return false;
	}
}
*/

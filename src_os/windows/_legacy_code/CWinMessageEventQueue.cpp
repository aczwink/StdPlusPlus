//Class header
#include "CWinMessageEventQueue.h"
//Global
#include <CommCtrl.h>
//Local
#include "../../../headers/UI/Views/C3DView.h"
#include "CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Global Variables
bool g_ignoreMessage;
LRESULT g_messageResult;

//Private functions
void CWinMessageEventQueue::DispatchControlEvent(AWidget &refWidget, UINT notificationCode)
{
	switch(notificationCode)
	{
	case BN_CLICKED:
		{
			if(IS_INSTANCE_OF(&refWidget, CPushButton))
			{
				CPushButton &refButton = (CPushButton &)refWidget;
				
				CEventQueue::DispatchPushedEvent(refButton);
			}
			else if(IS_INSTANCE_OF(&refWidget, CCheckBox))
			{
				CCheckBox &refCheckBox = (CCheckBox &)refWidget;
				
				CEventQueue::DispatchToggledEvent(refCheckBox);
			}
		}
		break;
	case CBN_SELCHANGE:
		{
			CDropDown &refDropDown = (CDropDown &)refWidget;
			
			CEventQueue::DispatchSelectionChangedEvent(refDropDown);
		}
		break;
	}
}

void CWinMessageEventQueue::DispatchNotificationEvent(AWidget &refWidget, const NMHDR &refNmHdr)
{
	switch(refNmHdr.code)
	{
	case TVN_SELCHANGEDW:
		{
			CTreeView &refTreeView = (CTreeView &)refWidget;

			CEventQueue::DispatchSelectionChangedEvent(refTreeView);
		}
		break;
	}
}

bool CWinMessageEventQueue::DispatchEvent(AWidget &refWidget, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_ignoreMessage = false;
	
	CWindow &refWnd = (CWindow &)refWidget;
	
	switch(message)
	{
	case WM_DESTROY:
		{
			//only forward to windows
			if(dynamic_cast<CWindow *>(&refWidget))
			{
				CEventQueue::DispatchDestroyEvent(refWnd);
				g_messageResult = 0;
			}
		}
		break;
	case WM_SIZE:
		{
			CEventQueue::DispatchResizedEvent(refWnd);
			g_messageResult = 0;
		}
		break;
	case WM_PAINT:
		{
			//very ugly to check this here like this
			if(dynamic_cast<C3DView *>(&refWidget))
			{
				RECT rcUpdate;
				
				//validate the update region (it is important that this is done before calling the event handler because user might want to redraw in paint handler!!!)
				GetUpdateRect(GET_HWND(&refWidget), &rcUpdate, false);
				ValidateRect(GET_HWND(&refWidget), &rcUpdate);
			}
			
			CEventQueue::DispatchPaintEvent(refWidget);
			g_messageResult = 0;
		}
		break;
	case WM_CLOSE:
		{
			CEventQueue::DispatchCloseEvent(refWnd);
			g_messageResult = 0;
		}
		break;
	case WM_NOTIFY:
		{
			const NMHDR *const& refpNmHdr = (NMHDR *)lParam;
			AWidget &refWidget = *(AWidget *)GetWindowLongPtr((HWND)refpNmHdr->hwndFrom, GWLP_USERDATA);
			
			DispatchNotificationEvent(refWidget, *refpNmHdr);
		}
		break;
	case WM_COMMAND:
		{
			g_messageResult = 0;

			if(lParam)
			{
				DispatchControlEvent(*(AWidget *)GetWindowLongPtr((HWND)lParam, GWLP_USERDATA), HIWORD(wParam));
			}
			else
			{
				/*
				if(HIGH16(wParam))
				{
					//Accelerator
					return false;
				}
				else
				{
					//Menu
					return false;
				}
				*/
				return false;
			}
		}
		break;
	case WM_MENUCOMMAND:
		{
			MENUINFO menuInfo;

			menuInfo.cbSize = sizeof(menuInfo);
			menuInfo.fMask = MIM_MENUDATA;
			GetMenuInfo((HMENU)lParam, &menuInfo);
			
			g_messageResult = 0; //THE DOCUMENTATION SAYS NOTHIGN ABOUT THE RETURN VALUE
		}
		break;
	default:
		return false;
	}

	return !g_ignoreMessage;
}

//Public functions
LRESULT CALLBACK CWinMessageEventQueue::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	AWidget *pWidget;
	
	if(message == WM_NCCREATE)
	{
		pWidget = (AWidget *)((LPCREATESTRUCT)lParam)->lpCreateParams;
		
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pWidget);
	}
	else
	{
		pWidget = (AWidget *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if(pWidget && CWinMessageEventQueue::DispatchEvent(*pWidget, message, wParam, lParam))
		return g_messageResult;

	return DefWindowProcW(hWnd, message, wParam, lParam);
}
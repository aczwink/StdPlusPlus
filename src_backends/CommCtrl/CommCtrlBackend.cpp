/*
* Copyright (c) 2018-2019,2021 Amir Czwink (amir130@hotmail.de)
*
* This file is part of Std++.
*
* Std++ is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Std++ is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Std++.  If not, see <http://www.gnu.org/licenses/>.
*/
//Class header
#include "CommCtrlBackend.hpp"
//Local
#include <Std++/UI/Style/StyleSheet.hpp>
#include "UI/CommCtrlCheckBoxBackend.hpp"
#include "UI/CommCtrlHeaderViewBackend.hpp"
#include "UI/CommCtrlLabelBackend.hpp"
#include "UI/CommCtrlPushButtonBackend.hpp"
#include "UI/CommCtrlRenderTargetWidgetBackend.hpp"
#include "UI/CommCtrlScrollAreaBackend.hpp"
#include "UI/CommCtrlSearchBoxBackend.hpp"
#include "UI/CommCtrlSelectBoxBackend.hpp"
#include "UI/CommCtrlSliderBackend.hpp"
#include "UI/CommCtrlSpinBoxBackend.hpp"
#include "UI/CommCtrlTableViewBackend.hpp"
#include "UI/CommCtrlTreeViewBackend.hpp"
#include "UI/CommCtrlWindowBackend.hpp"
#include "UI/WindowsMessageQueueEventSource.hpp"
#include "CommCtrlOpenGL3CoreBackend.hpp"
#include "UI/CommCtrlGroupBoxBackend.hpp"
#include "UI/Win32DrawableWidget.hpp"
//Namespaces
using namespace _stdxx_;

//Constructor
CommCtrlBackend::CommCtrlBackend()
{
#ifdef _STDPLUSPLUS_BACKEND_OPENGL
	CommCtrlOpenGL3CoreBackend *commCtrlOpenGL3CoreBackend = new CommCtrlOpenGL3CoreBackend;
	this->renderBackends.RegisterBackend(commCtrlOpenGL3CoreBackend, 0);
#endif
}

//Public methods
CheckBoxBackend *CommCtrlBackend::CreateCheckBoxBackend(UI::CheckBox *checkBox)
{
	return new CommCtrlCheckBoxBackend(*this, checkBox);
}

DrawableWidgetBackend* CommCtrlBackend::CreateDrawableWidgetBackend(UI::Widget & widget)
{
	return new Win32DrawableWidget(*this, widget);
}

EventHandling::EventSource *CommCtrlBackend::CreateEventSource()
{
	return new WindowsMessageQueueEventSource;
}

GroupBoxBackend *CommCtrlBackend::CreateGroupBoxBackend(UI::GroupBox& groupBox)
{
	return new CommCtrlGroupBoxBackend(*this, &groupBox);
}

HeaderViewBackend * CommCtrlBackend::CreateHeaderViewBackend(UI::HeaderView & headerView)
{
	return new CommCtrlHeaderViewBackend(*this, headerView);
}

LabelBackend *CommCtrlBackend::CreateLabelBackend(UI::Label& label)
{
	return new CommCtrlLabelBackend(*this, &label);
}

MenuBarBackend *CommCtrlBackend::CreateMenuBarBackend(UI::MenuBar *menuBar)
{
	return new CommCtrlMenuBarBackend(menuBar);
}

PushButtonBackend *CommCtrlBackend::CreatePushButtonBackend(UI::PushButton& pushButton)
{
	return new CommCtrlPushButtonBackend(*this, &pushButton);
}

WidgetBackend *CommCtrlBackend::CreateRenderTargetWidgetBackend(UI::RenderTargetWidget& renderTargetWidget)
{
	return new CommCtrlRenderTargetWidgetBackend(*this, &renderTargetWidget);
}

SliderBackend *CommCtrlBackend::CreateSliderBackend(UI::Slider& slider)
{
	return new CommCtrlSliderBackend(*this, &slider);
}

SpinBoxBackend *CommCtrlBackend::CreateSpinBoxBackend(UI::SpinBox *spinBox)
{
	return new CommCtrlSpinBoxBackend(*this, spinBox);
}

ContentAreaWidgetBackend* CommCtrlBackend::CreateScrollAreaBackend(UI::ScrollArea * scrollArea)
{
	return new CommCtrlScrollAreaBackend(*this, scrollArea);
}

WidgetBackend * CommCtrlBackend::CreateSearchBoxBackend(UI::SearchBox & searchBox)
{
	return new CommCtrlSearchBoxBackend(*this, searchBox);
}

ViewBackend * CommCtrlBackend::CreateSelectBoxBackend(UI::SelectBox & selectBox)
{
	return new CommCtrlSelectBoxBackend(*this, selectBox);
}

ViewBackend * CommCtrlBackend::CreateTableViewBackend(UI::TableView & tableView)
{
	return new CommCtrlTableViewBackend(*this, tableView);
}

ViewBackend * CommCtrlBackend::CreateTreeViewBackend(UI::TreeView & treeView)
{
	//the native tree view control does not support multiple column
	//return new CommCtrlTreeViewBackend(*this, treeView);
	return nullptr;
}

WindowBackend * CommCtrlBackend::CreateWindowBackend(UI::Window* window)
{
	return new CommCtrlWindowBackend(*this, window);
}

void CommCtrlBackend::Load()
{
	//enable visual styles
	WCHAR dir[MAX_PATH];
	ACTCTXW actCtx =
	{
		sizeof(actCtx),
		ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_SET_PROCESS_DEFAULT | ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID, L"shell32.dll", 0, 0, dir, (LPCWSTR)124
	};
	UINT cch = GetSystemDirectoryW(dir, sizeof(dir) / sizeof(*dir));
	ASSERT( !(cch >= sizeof(dir) / sizeof(*dir)), u8"REPORT THIS PLEASE!");
	dir[cch] = 0;
	this->hActCtx = CreateActCtxW(&actCtx);
	ActivateActCtx(this->hActCtx, &this->ulpActivationCookie);

	//init control library
	INITCOMMONCONTROLSEX iccex;

	iccex.dwSize = sizeof(iccex);
	iccex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES | ICC_TAB_CLASSES | ICC_UPDOWN_CLASS | ICC_LINK_CLASS | ICC_STANDARD_CLASSES;

	DWORD err = GetLastError();
	BOOL result = InitCommonControlsEx(&iccex);
	err = GetLastError();
	ASSERT(result == TRUE, u8"Could not initialize common controls library.");

	//register window class
	WNDCLASSEXW wcex = { 0 };
	//wcex.style = CS_OWNDC;
	wcex.cbSize = sizeof(wcex);
	wcex.hInstance = GetModuleHandle(NULL);;
	wcex.lpfnWndProc = WindowsMessageQueueEventSource::WndProc;
	wcex.lpszClassName = STDPLUSPLUS_WIN_WNDCLASS;

	ATOM classAtom = RegisterClassExW(&wcex);
	ASSERT(classAtom != 0, u8"Could not register window class");

	this->LoadStyles();
}

void CommCtrlBackend::Unload() const
{
	//unregister window class
	if (!UnregisterClassW(STDPLUSPLUS_WIN_WNDCLASS, GetModuleHandle(NULL)))
	{
		DWORD lastErr;

		lastErr = GetLastError();
		switch (lastErr)
		{
		case ERROR_CLASS_DOES_NOT_EXIST:
		{
			NOT_IMPLEMENTED_ERROR; //for some weird reason the class is sometimes not registered... i think when the program crashes..
			//this should be checked
		}
		break;
		case ERROR_CLASS_HAS_WINDOWS:
		{
			ASSERT(false, "You didn't close all windows");
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR;
		}
	}

	//release activation context
	DeactivateActCtx(0, this->ulpActivationCookie);
	ReleaseActCtx(this->hActCtx);
}

//Private methods
void CommCtrlBackend::LoadStyles() const
{
	StyleSheet s;

	//set theme vars
	HTHEME hTheme = OpenThemeData(NULL, L"Window");
	s.SetVariable(u8"backgroundColor", this->MapCOLORREF(hTheme, COLOR_WINDOW));
	s.SetVariable(u8"textColor", this->MapCOLORREF(hTheme, COLOR_WINDOWTEXT));
	s.SetVariable(u8"selectionBackgroundColor", this->MapCOLORREF(hTheme, COLOR_HIGHLIGHT));
	s.SetVariable(u8"selectionTextColor", this->MapCOLORREF(hTheme, COLOR_HIGHLIGHTTEXT));
	CloseThemeData(hTheme);

	//load styles
	String c_style = {
#include "resources/win.css"
	};
	s.Parse(c_style);

	//set global styles
	StyleSheet::Global() = Move(s);
}

Color CommCtrlBackend::MapCOLORREF(HTHEME hTheme, int colorId) const
{
	COLORREF cr = GetThemeSysColor(hTheme, colorId);
	Color c;
	c.r = GetRValue(cr) / 255.0f;
	c.g = GetGValue(cr) / 255.0f;
	c.b = GetBValue(cr) / 255.0f;

	return c;
}

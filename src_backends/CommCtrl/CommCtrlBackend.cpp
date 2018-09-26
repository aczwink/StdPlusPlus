/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "UI/CommCtrlCheckBoxBackend.hpp"
#include "UI/CommCtrlLabelBackend.hpp"
#include "UI/CommCtrlPushButtonBackend.hpp"
#include "UI/CommCtrlRenderTargetWidgetBackend.hpp"
#include "UI/CommCtrlScrollAreaBackend.hpp"
#include "UI/CommCtrlSliderBackend.hpp"
#include "UI/CommCtrlSpinBoxBackend.hpp"
#include "UI/CommCtrlWindowBackend.hpp"
#include "UI/WindowsMessageQueueEventSource.hpp"
#include "CommCtrlOpenGL3CoreBackend.hpp"
#include "UI/CommCtrlGroupBoxBackend.hpp"
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
	return new CommCtrlCheckBoxBackend(this, checkBox);
}

EventSource *CommCtrlBackend::CreateEventSource()
{
	return new WindowsMessageQueueEventSource;
}

GroupBoxBackend *CommCtrlBackend::CreateGroupBoxBackend(UI::GroupBox * groupBox)
{
	return new CommCtrlGroupBoxBackend(this, groupBox);
}

LabelBackend *CommCtrlBackend::CreateLabelBackend(UI::Label *label)
{
	return new CommCtrlLabelBackend(this, label);
}

MenuBarBackend *CommCtrlBackend::CreateMenuBarBackend(UI::MenuBar *menuBar)
{
	return new CommCtrlMenuBarBackend(menuBar);
}

PushButtonBackend *CommCtrlBackend::CreatePushButtonBackend(UI::PushButton * pushButton)
{
	return new CommCtrlPushButtonBackend(this, pushButton);
}

WidgetBackend *CommCtrlBackend::CreateRenderTargetWidgetBackend(UI::RenderTargetWidget * renderTargetWidget)
{
	return new CommCtrlRenderTargetWidgetBackend(this, renderTargetWidget);
}

SliderBackend *CommCtrlBackend::CreateSliderBackend(UI::Slider *slider)
{
	return new CommCtrlSliderBackend(this, slider);
}

SpinBoxBackend *CommCtrlBackend::CreateSpinBoxBackend(UI::SpinBox *spinBox)
{
	return new CommCtrlSpinBoxBackend(this, spinBox);
}

ContentAreaWidgetBackend* CommCtrlBackend::CreateScrollAreaBackend(UI::ScrollArea * scrollArea)
{
	return new CommCtrlScrollAreaBackend(this, scrollArea);
}

WindowBackend * CommCtrlBackend::CreateWindowBackend(UI::Window * window)
{
	return new CommCtrlWindowBackend(this, window);
}

void CommCtrlBackend::Load()
{
	//init control library
	INITCOMMONCONTROLSEX iccex;

	iccex.dwSize = sizeof(iccex);
	iccex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES | ICC_TAB_CLASSES | ICC_UPDOWN_CLASS | ICC_LINK_CLASS | ICC_STANDARD_CLASSES;

	ASSERT(InitCommonControlsEx(&iccex), u8"Could not initialize common controls library.");

	//register window class
	WNDCLASSEXW wcex = { 0 };
	//wcex.style = CS_OWNDC;
	wcex.cbSize = sizeof(wcex);
	wcex.hInstance = GetModuleHandle(NULL);;
	wcex.lpfnWndProc = WindowsMessageQueueEventSource::WndProc;
	wcex.lpszClassName = STDPLUSPLUS_WIN_WNDCLASS;

	ASSERT(RegisterClassExW(&wcex), u8"Could not register window class");
}
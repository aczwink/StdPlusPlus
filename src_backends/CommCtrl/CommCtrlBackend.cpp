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
#include "UI/CommCtrlPushButtonBackend.hpp"
#include "UI/CommCtrlRenderTargetWidgetBackend.hpp"
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
_stdxx_::CheckBoxBackend * StdXX::CommCtrlBackend::CreateCheckBoxBackend(UI::CheckBox * checkBox)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

EventSource *CommCtrlBackend::CreateEventSource()
{
	return new WindowsMessageQueueEventSource;
}

_stdxx_::GroupBoxBackend * StdXX::CommCtrlBackend::CreateGroupBoxBackend(UI::GroupBox * groupBox)
{
	return new CommCtrlGroupBoxBackend(this, groupBox);
}

_stdxx_::LabelBackend * StdXX::CommCtrlBackend::CreateLabelBackend(UI::Label * label)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
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

_stdxx_::SliderBackend * StdXX::CommCtrlBackend::CreateSliderBackend(UI::Slider * slider)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

_stdxx_::SpinBoxBackend * StdXX::CommCtrlBackend::CreateSpinBoxBackend(UI::SpinBox * spinBox)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
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
	iccex.dwICC = ICC_BAR_CLASSES | ICC_LINK_CLASS | ICC_LISTVIEW_CLASSES | ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES | ICC_UPDOWN_CLASS;

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
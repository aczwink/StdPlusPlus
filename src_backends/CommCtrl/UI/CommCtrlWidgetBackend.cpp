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
#include "CommCtrlWidgetBackend.hpp"
//Local
#include <Std++/UI/WidgetContainer.hpp>
#include <Std++/UI/Window.hpp>
#include "Win32Window.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
const Win32Window* CommCtrlWidgetBackend::GetNextWindow(const Win32Window* window) const
{
	if (window == nullptr)
		return dynamic_cast<const Win32Window*>(this);
	return nullptr;
}

void CommCtrlWidgetBackend::IgnoreEvent()
{
	extern bool g_ignoreMessage;
	g_ignoreMessage = true;
}

void CommCtrlWidgetBackend::PrePaint()
{
}

void CommCtrlWidgetBackend::Repaint()
{
	Win32Window *thisWnd = dynamic_cast<Win32Window *>(this);
	InvalidateRect(thisWnd->GetHWND(), nullptr, false);
}

void CommCtrlWidgetBackend::SetBounds(const RectD &bounds)
{
	Win32Window *thisWnd = dynamic_cast<Win32Window *>(this);
	if (thisWnd)
		thisWnd->SetRect(this->ToWinAPIBounds(bounds));
}

void CommCtrlWidgetBackend::SetEnabled(bool enable)
{
	Win32Window *thisWnd = dynamic_cast<Win32Window *>(this);
	thisWnd->SetEnabled(enable);
}

void CommCtrlWidgetBackend::Show(bool visible)
{
	Win32Window *thisWnd = dynamic_cast<Win32Window *>(this);
	thisWnd->Show(visible);
}

//Protected methods
RectD CommCtrlWidgetBackend::ToWinAPIBounds(const RectD &bounds) const
{
	/*
	we basically need to only "invert" the y axis of the origin.
	It should be kept in mind that the origin is defined inside the bounds (client rect) of the parent! (i.e. the parent with a commctrl backend!!!)
	*/
	//find parent
	const WidgetContainer* parent = this->GetWidget().GetParent();
	while (parent->_GetBackend() == nullptr) //skip only virtual containers (i.e. without backend)
		parent = parent->GetParent();

	//parent should be a ContainerWidget
	ASSERT(parent->GetNumberOfChildren() == 1, u8"TODO: report this please");
	const WidgetContainer* container = dynamic_cast<const WidgetContainer*>(parent->GetChild(0));

	//transform coordinates
	PointD origin = bounds.origin;
	origin.y = bounds.GetVerticalEnd(); //compute "top"
	origin = this->GetWidget().GetParent()->TranslateToAncestorCoords(origin, container); //bounds.origin is defined in parent coordinates
																						  
	//"invert" y axis
	const Win32Window* wnd = dynamic_cast<const Win32Window*>(parent->_GetBackend());
	ASSERT(wnd, u8"TODO: report this please");
	origin.y = wnd->GetClientRect().height() - origin.y;
	
	return RectD(origin, bounds.size);
}
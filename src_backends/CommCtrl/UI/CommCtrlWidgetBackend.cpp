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
void CommCtrlWidgetBackend::PrePaint()
{
}

void CommCtrlWidgetBackend::Reparent(Win32Window *newParent)
{
	Win32Window *thisWnd = dynamic_cast<Win32Window *>(this);
	thisWnd->SetParent(newParent);
}

void CommCtrlWidgetBackend::SetBounds(const RectD &bounds)
{
	Win32Window *thisWnd = dynamic_cast<Win32Window *>(this);
	if (thisWnd)
		thisWnd->SetRect(this->ToWinAPIBounds(bounds));
}

//Protected methods
RectD CommCtrlWidgetBackend::ToWinAPIBounds(const RectD &bounds) const
{
	PointD origin = this->TransformToWindow(PointD()); //we want the root point (0, 0) of the widgets local coordinates

	//origin is in content area coordinates of window, i.e. in "client coordinates" (but with bottom-left corner)
	const RectD &basis = this->GetWidget().GetWindow()->GetContentContainer()->GetBounds();
	//compute the top of the rect
	origin.y += bounds.size.height;
	//invert y axis
	origin.y = basis.size.height - origin.y;
	
	return RectD(origin, bounds.size);
}

PointD CommCtrlWidgetBackend::TransformToWindow(const PointD &point) const
{
	PointD transformed = point;

	const Window *window = this->GetWidget().GetWindow();
	const Widget *current = &this->GetWidget();
	while (current->GetParent() != window)
	{
		transformed += current->GetBounds().origin;
		current = current->GetParent();
	}

	return  transformed;
}
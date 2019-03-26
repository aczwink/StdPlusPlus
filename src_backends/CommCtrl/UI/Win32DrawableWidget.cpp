/*
* Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include "Win32DrawableWidget.hpp"
//Local
#include <Std++/UI/Widget.hpp>
#include "Win32Painter.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
UniquePointer<Painter> Win32DrawableWidget::CreatePainter()
{
	return new Win32Painter(*this);
}

SizeD Win32DrawableWidget::GetSizeHint() const
{
	return StdXX::Math::SizeD();
}

void Win32DrawableWidget::OnMessage(WinMessageEvent& event)
{
	switch (event.message)
	{
	case WM_PAINT:
	{
		RECT rc;
		GetUpdateRect(this->GetHWND(), &rc, FALSE);
		RectD updateRect;
		updateRect.x() = rc.left;
		updateRect.width() = rc.right - rc.left;
		updateRect.height() = rc.bottom - rc.top;
		updateRect.y() = this->widget.GetSize().height - rc.bottom;
		
		PaintEvent paintEvent(updateRect);
		this->widget.Event(paintEvent);

		event.consumed = paintEvent.WasAccepted();
		event.result = 0;
	}
	break;
	}
}

void _stdxx_::Win32DrawableWidget::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR;
}
/*
* Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
*
* This file is part of ACStdLib.
*
* ACStdLib is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ACStdLib is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
*/
//Class header
#include <ACStdLib/UI/Widget.hpp>
//Global
#include <Windows.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Global Variables
extern bool g_ignoreMessage;

//Proctected functions
void Widget::IgnoreEvent()
{
	g_ignoreMessage = true;
}

//Public methods
void Widget::Repaint()
{
	if(this->backend)
	{
		InvalidateRect((HWND)this->backend, nullptr, false);
	}
	else
	{
		this->OnPaint();
	}
}

/*
 * void Widget::SetEnabled(bool enable)
-{
-	EnableWindow((HWND)this->systemHandle, enable);
-}
-
-void Widget::Show(bool visible)
-{
-	ShowWindow((HWND)this->systemHandle, visible ? SW_SHOW : SW_HIDE);

 -Size Widget::System_GetSize() const
-{
-	RECT rc;
-
-	GetWindowRect((HWND)this->systemHandle, &rc);
-
-	return Size(rc.right - rc.left, rc.bottom - rc.top);
-}
-
-void Widget::System_SetRect(const Rect &area)
-{
-	Point transformed;
-
-	transformed = this->TransformToWindow(area.origin);
-	SetWindowPos((HWND)this->systemHandle, HWND_TOP, transformed.x, transformed.y, area.width(), area.height(), 0);
-}
 */
/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Keyboard.hpp>
//Global
#include <gdk/gdkx.h>
//Local
#include <Std++/Debug.h>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Local functions
KeySym MapToXKeyCode(UI::KeyCode keyCode)
{
	switch(keyCode)
	{
		case UI::KeyCode::Shift_Left:
			return XK_Shift_L;
		case UI::KeyCode::Shift_Right:
			return XK_Shift_R;
		case UI::KeyCode::A:
			return XK_A;
		case UI::KeyCode::D:
			return XK_D;
		case UI::KeyCode::S:
			return XK_S;
		case UI::KeyCode::W:
			return XK_W;
	}

	NOT_IMPLEMENTED_ERROR;
}

//Global functions
bool UI::GetKeyState(UI::KeyCode keyCode)
{
	GdkDisplay *display = gdk_display_get_default();
	Display *xDisplay = gdk_x11_display_get_xdisplay(display);

	char szKey[32];
	int iKeyCodeToFind = XKeysymToKeycode(xDisplay, MapToXKeyCode(keyCode));
	XQueryKeymap(xDisplay, szKey);
	int down = szKey[iKeyCodeToFind / 8] & (1 << (iKeyCodeToFind % 8));

	return down != 0;
}
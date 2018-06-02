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
#include <Std++/UI/Mouse.hpp>
//Global
#include <gtk/gtk.h>
//Local
#include <Std++/Math/Size.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Global functions
void UI::GetMousePointerPos(uint16 &x, uint16 &y)
{
	GdkDisplay *display = gdk_display_get_default();
	GdkSeat *seat = gdk_display_get_default_seat(display);
	GdkDevice *device = gdk_seat_get_pointer(seat);

	gint gx, gy;
	gdk_device_get_position(device, nullptr, &gx, &gy);
	x = static_cast<uint16>(gx);
	y = static_cast<uint16>(Size::GetScreenSize().height - gy);
}

bool UI::GetMouseButtonState(MouseButton button)
{
	GdkDisplay *display = gdk_display_get_default();
	GdkSeat *seat = gdk_display_get_default_seat(display);
	GdkDevice *device = gdk_seat_get_pointer(seat);

	GdkModifierType modifiers;
	gdk_device_get_state(device, gdk_get_default_root_window(), nullptr, &modifiers);

	switch(button)
	{
		case MouseButton::Left:
			return modifiers & GDK_BUTTON1_MASK;
		case MouseButton::Right:
			return modifiers & GDK_BUTTON3_MASK;
	}
	
	return false;
}
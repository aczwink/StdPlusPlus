/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
//Local
#include "XConnection.hpp"

namespace _stdxx_
{
	class XcbWindow
	{
	public:
		//Constructor
		inline XcbWindow(XConnection& xConnection, xcb_visualid_t visualId) : connection(xConnection.Connection())
		{
			this->windowId = xcb_generate_id(this->connection);

			xcb_create_window(this->connection, XCB_COPY_FROM_PARENT, this->windowId, xConnection.GetDefaultScreen()->root,
					0, 0, 150, 150, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
					visualId, 0, nullptr);
		}

		//Inline
		inline void Map(bool map)
		{
			if(map)
				xcb_map_window(this->connection, this->windowId);
			else
				xcb_unmap_window(this->connection, this->windowId);
		}

		inline StdXX::Math::Rect<int32> GetGeometry() const
		{
			xcb_get_geometry_cookie_t cookie = xcb_get_geometry(this->connection, this->windowId);
			xcb_generic_error_t* error;
			xcb_get_geometry_reply_t* result = xcb_get_geometry_reply(this->connection, cookie, &error);
			ASSERT(error == nullptr, u8"REPORT THIS PLEASE!");

			StdXX::Math::Rect<int32> rect(result->x, result->y, result->width, result->height);
			free(result);

			return rect;
		}

		inline void SetGeometry(const StdXX::Math::Rect<int32>& rect)
		{
			const static int32 values[] = {rect.x(), rect.y(), rect.width(), rect.height()};
			xcb_configure_window(this->connection, this->windowId, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
					XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
		}

		inline void SetTitle(const StdXX::String& title)
		{
			xcb_change_property(this->connection, XCB_PROP_MODE_REPLACE, this->windowId, XCB_ATOM_WM_NAME, XCB_ATOM_STRING,
					8, title.GetLength(), title.ToUTF8().GetRawZeroTerminatedData());
		}

	private:
		//Members
		xcb_connection_t* connection;
		xcb_window_t windowId;
	};
}
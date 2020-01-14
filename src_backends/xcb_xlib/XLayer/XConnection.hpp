/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include <Std++/Debug.hpp>

#include "Imports.hpp"

namespace _stdxx_
{
	class XConnection
	{
	public:
		//Constructor
		inline XConnection()
		{
			this->display = XOpenDisplay(nullptr);
			ASSERT(this->display, u8"REPORT THIS PLEASE!");

			this->connection = XGetXCBConnection(this->display);

			//find default screen
			int defaultScreenNumber = DefaultScreen(this->display);

			XSetEventQueueOwner(this->display, XCBOwnsEventQueue);

			xcb_screen_iterator_t screenIterator = xcb_setup_roots_iterator(xcb_get_setup(this->connection));
			for(int screenNum = defaultScreenNumber; screenIterator.rem && (screenNum > 0); screenNum--)
				xcb_screen_next(&screenIterator);
			defaultScreen = screenIterator.data;
		}

		//Destructpr
		inline ~XConnection()
		{
			XCloseDisplay(this->display);
		}

		//Properties
		inline xcb_connection_t* Connection()
		{
			return this->connection;
		}

		//Inline
		inline const xcb_screen_t* GetDefaultScreen() const
		{
			return this->defaultScreen;
		}

		inline Display* GetDisplay()
		{
			return this->display;
		}

	private:
		//Members
		Display* display;
		xcb_connection_t* connection;
		xcb_screen_t* defaultScreen;
	};
}
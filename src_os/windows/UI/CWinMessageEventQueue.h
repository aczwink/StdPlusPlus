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
#pragma once
//Global
#include <Windows.h>
//Local
#include <Std++/UI/EventQueue.hpp>

class CWinMessageEventQueue : public StdPlusPlus::UI::EventQueue
{
private:
	//Functions
	static void DispatchControlEvent(StdPlusPlus::UI::Widget &refWidget, UINT notificationCode);
	static void DispatchNotificationEvent(StdPlusPlus::UI::Widget &refWidget, const NMHDR &refNmHdr);
	//Return value: true if message was processed, false if not.
	static bool DispatchEvent(StdPlusPlus::UI::Widget &refWidget, UINT message, WPARAM wParam, LPARAM lParam);
public:
	//Functions
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
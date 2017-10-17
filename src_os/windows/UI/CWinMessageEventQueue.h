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
#pragma once
//Global
#include <Windows.h>
//Local
#include <ACStdLib/UI/EventQueue.hpp>

class CWinMessageEventQueue : public ACStdLib::UI::EventQueue
{
private:
	//Functions
	static void DispatchControlEvent(ACStdLib::UI::Widget &refWidget, UINT notificationCode);
	static void DispatchNotificationEvent(ACStdLib::UI::Widget &refWidget, const NMHDR &refNmHdr);
	//Return value: true if message was processed, false if not.
	static bool DispatchEvent(ACStdLib::UI::Widget &refWidget, UINT message, WPARAM wParam, LPARAM lParam);
public:
	//Functions
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
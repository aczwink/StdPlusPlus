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
#include <Std++/UI/UIEventSource.hpp>
#include <Std++/UI/Widget.hpp>

class WindowsMessageQueueEventSource : public StdPlusPlus::UI::UIEventSource
{
public:
	//Constructor
	WindowsMessageQueueEventSource();

	//Methods
	void DispatchPendingEvents() override;
	uint64 GetMaxTimeout() const override;

	//Functions
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	//Methods
	void DispatchControlEvent(StdPlusPlus::UI::Widget &widget, UINT notificationCode);
	/**
	 *
	 * @param widget
	 * @param message
	 * @param wParam
	 * @param lParam
	 * @return true if message was processed, false if not.
	 */
	bool DispatchMessageEvent(StdPlusPlus::UI::Widget &widget, UINT message, WPARAM wParam, LPARAM lParam);
	void DispatchNotificationEvent(StdPlusPlus::UI::Widget &refWidget, const NMHDR &refNmHdr);
};
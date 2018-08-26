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
//Local
#include "Definitions.h"
#include <Std++/Function.hpp>
#include <Std++/UI/UIEventSource.hpp>

//Forward declarations
namespace _stdxx_
{
	class CommCtrlWindowBackend;
}

class WindowsMessageQueueEventSource : public StdXX::UI::UIEventSource
{
public:
	//Constructor
	WindowsMessageQueueEventSource();

	//Methods
	void DispatchPendingEvents() override;
	uint64 GetMaxTimeout() const override;
	void VisitWaitObjects(const StdXX::Function<void (_stdxx_::WaitObjHandle, bool)>& visitFunc) const override;

	//Functions
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	//Methods
	void DispatchControlEvent(_stdxx_::CommCtrlWindowBackend &backend, UINT notificationCode);
	/**
	 *
	 * @param widget
	 * @param message
	 * @param wParam
	 * @param lParam
	 * @return true if message was processed, false if not.
	 */
	bool DispatchMessageEvent(_stdxx_::WidgetBackend &backend, UINT message, WPARAM wParam, LPARAM lParam);
	void DispatchNotificationEvent(StdXX::UI::Widget &refWidget, const NMHDR &refNmHdr);
};
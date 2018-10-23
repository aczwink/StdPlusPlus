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
#include "../Imports.h"
#include <Std++/Function.hpp>
#include <Std++/UI/UIEventSource.hpp>

//Forward declarations
namespace _stdxx_
{
	class CommCtrlWidgetBackend;
}

class WindowsMessageQueueEventSource : public StdXX::UI::UIEventSource
{
public:
	//Constructor
	WindowsMessageQueueEventSource();

	//Methods
	void DispatchPendingEvents() override;
	uint64 GetMaxTimeout() const override;
	void VisitWaitObjects(const StdXX::Function<void (_stdxx_::WaitObjHandle, bool)>& visitFunc) override;

	//Functions
	static _stdxx_::CommCtrlWidgetBackend *GetAttachedBackend(HWND hWnd);
	static _stdxx_::CommCtrlWidgetBackend *GetAttachedBackendIfStdXXWnd(HWND hWnd);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	//Methods
	void DispatchControlEvent(_stdxx_::CommCtrlWidgetBackend &backend, UINT notificationCode);
	void DispatchNotificationEvent(StdXX::UI::Widget &refWidget, const NMHDR &refNmHdr);
};
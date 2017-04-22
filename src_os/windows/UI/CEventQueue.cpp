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
#include "../../../headers/UI/CEventQueue.h"
//Global
#include <Windows.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Protected functions
void CEventQueue::DispatchPaintEvent(AWidget &refWidget)
{
	refWidget.OnPaint();
}

//Public functions
void CEventQueue::PostQuitEvent(int32 exitCode)
{
	PostQuitMessage(exitCode);
}

int32 CEventQueue::ProcessEvents()
{
	MSG msg;

	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		//TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int32)msg.wParam;
}

bool CEventQueue::ProcessPendingEvents(int32 &refExitCode)
{
	MSG msg;

	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT)
		{
			refExitCode = (int32)msg.wParam;
			return false;
		}

		DispatchMessage(&msg);
	}

	return true;
}
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
//Local
#include <ACStdLib/UI/Widget.hpp>
//Global
#include <Windows.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

class CFullAccessWidget : public Widget
{
public:
	//Inline
	inline HWND GetHWND()
	{
		return (HWND)this->systemHandle;
	}
};

#define GET_HWND(pWidget) (((CFullAccessWidget *)pWidget)->GetHWND())
#define WANT_OS_HANDLED(pParent) (GET_HWND(pParent) != nullptr)
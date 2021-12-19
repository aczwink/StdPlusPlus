/*
* Copyright (c) 2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Strings/String.hpp>

//Forward declarations
namespace StdXX
{
	namespace UI
	{
		class ActionEntry;
		class Menu;
	}
}

namespace _stdxx_
{
	class MenuBackend
	{
	public:
		//Destructor
		virtual ~MenuBackend() {}

		//Abstract
		virtual void AppendEntry(const StdXX::UI::ActionEntry &actionEntry) = 0;
		virtual void AppendSeperator() = 0;
		virtual void AppendSubMenu(StdXX::UI::Menu *subMenu) = 0;
		virtual void SetText(const StdXX::String &text) = 0;
		virtual void ShowPopup() = 0;
	};
}
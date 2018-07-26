/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/UI/MenuBackend.hpp>
#include <Std++/UI/Menu/Menu.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

namespace _stdpp
{
	class CommCtrlMenuBackend : public MenuBackend
	{
	public:
		//Constructor
		CommCtrlMenuBackend(StdPlusPlus::UI::Menu *menu);

		//Destructor
		~CommCtrlMenuBackend();

		//Methods
		void AppendEntry(const StdPlusPlus::UI::ActionEntry &actionEntry) override;
		void AppendSeperator() override;
		void AppendSubMenu(StdPlusPlus::UI::Menu *subMenu) override;
		StdPlusPlus::String GetText() const override;
		void SetText(const StdPlusPlus::String & text) override;

		//Inline
		inline const HMENU GetHandle() const
		{
			return this->hMenu;
		}

	private:
		//Members
		Menu *menu;
		HMENU hMenu;
		String text;
	};
}
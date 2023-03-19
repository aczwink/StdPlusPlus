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
//Class header
#include <Std++/UI/Menu/MenuBar.hpp>
//Local
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/UI/Menu/Menu.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
MenuBar::MenuBar()
{
	this->backend = BackendManager<UIBackend>::GetRootInstance().GetActiveBackend()->CreateMenuBarBackend(this);
}

//Destructor
MenuBar::~MenuBar()
{
	for (Menu *const& refpMenu : this->attachedMenus)
		delete refpMenu;
	delete this->backend;
}

//Public methods

//Private methods
void MenuBar::EnsureMenuIsRealized(Menu& menu)
{
	if(menu.backend != nullptr)
		return;

	menu.backend = BackendManager<UIBackend>::GetRootInstance().GetActiveBackend()->CreateMenuBackend(menu);
	menu.backend->SetText(menu.text);

	for(const auto& entry : menu.menuEntries)
	{
		const ActionEntry* actionEntry = dynamic_cast<const ActionEntry *>(entry);
		if(actionEntry)
		{
			menu.backend->AppendEntry(*actionEntry);
		}
	}
}
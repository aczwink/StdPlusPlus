/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Windows/Dialog.hpp>
//Local
#include <Std++/_Backends/UI/DialogBackend.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/UI/Windows/Window.hpp>
//Namespaces
using namespace StdXX::UI;

//Public methods
bool Dialog::Run(Window* parent, Property<bool>& isValid)
{
	this->parent = parent;

	this->EnsureRealized();
	this->Show(true);
	bool result = this->dialogBackend->Run(isValid);
	this->Show(false);

	return result;
}

//Private methods
void Dialog::RealizeSelf()
{
	auto* uiBackend = (this->parent == nullptr) ? BackendManager<UIBackend>::GetRootInstance().GetActiveBackend() : &this->parent->_GetBackend()->GetUIBackend();

	this->dialogBackend = uiBackend->CreateDialogBackend(*this);
	this->_SetBackend(this->dialogBackend);
}

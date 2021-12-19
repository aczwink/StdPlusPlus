/*
 * Copyright (c) 2017-2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Controls/CheckBox.hpp>
//Local
#include <Std++/_Backends/UI/CheckBoxBackend.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/UI/Events/ValueChangedEvent.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
CheckBox::CheckBox() : checkBoxBackend(nullptr), isChecked(false)
{
	this->sizingPolicy.SetHorizontalPolicy(SizingPolicy::Policy::Minimum);
	this->sizingPolicy.SetVerticalPolicy(SizingPolicy::Policy::Fixed);
}

//Public methods
void CheckBox::Event(UI::Event& event)
{
	switch (event.GetType())
	{
		case EventType::ValueChanged:
		{
			ValueChangedEvent& vce = static_cast<ValueChangedEvent&>(event);

			this->isChecked = vce.GetNewValue().b;
			this->toggled.Emit();

			event.Accept();
		}
		break;
		default:
			Widget::Event(event);
	}
}

//Private methods
void CheckBox::RealizeSelf()
{
	_stdxx_::CheckBoxBackend* checkBoxBackend = this->_GetUIBackend()->CreateCheckBoxBackend(*this);
	this->_SetBackend(checkBoxBackend);
}

//Event handlers
void CheckBox::OnRealized()
{
	Widget::OnRealized();

	this->checkBoxBackend->UpdateCheckState();
	this->checkBoxBackend->SetText(this->text);
}
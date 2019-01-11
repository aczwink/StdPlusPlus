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
#include <Std++/UI/Controls/SpinBox.hpp>
//Local
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/UI/Events/ValueChangedEvent.hpp>
#include <Std++/Signed.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
SpinBox::SpinBox() : min(Signed<int32>::Min()), max(Signed<int32>::Max()), value(0), spinBoxBackend(nullptr)
{
	this->sizingPolicy.SetHorizontalPolicy(SizingPolicy::Policy::Minimum);
	this->sizingPolicy.SetVerticalPolicy(SizingPolicy::Policy::Fixed);
}

//Public methods
void SpinBox::Event(UI::Event& event)
{
	switch (event.GetType())
	{
	case EventType::ValueChanged:
	{
		ValueChangedEvent& vce = static_cast<ValueChangedEvent&>(event);

		this->value = vce.GetNewValue().i32;
		if (this->onValueChangedHandler.IsBound())
			this->onValueChangedHandler();

		event.Accept();
	}
	break;
	default:
		Widget::Event(event);
	}
}

//Private methods
void SpinBox::RealizeSelf()
{
	_stdxx_::SpinBoxBackend* spinBoxBackend = this->_GetUIBackend()->CreateSpinBoxBackend(this);
	this->_SetBackend(spinBoxBackend);
}

//Event handlers
void SpinBox::OnRealized()
{
	Widget::OnRealized();

	this->spinBoxBackend->SetRange(this->min, this->max);
	this->spinBoxBackend->SetValue(this->value);
}
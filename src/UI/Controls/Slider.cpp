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
#include <Std++/UI/Controls/Slider.hpp>
//Local
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/UI/Events/ValueChangedEvent.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
Slider::Slider() : sliderBackend(nullptr)
{
	this->sizingPolicy.SetHorizontalPolicy(SizingPolicy::Policy::Expanding);
	this->sizingPolicy.SetVerticalPolicy(SizingPolicy::Policy::Fixed);

	this->min = 0;
	this->max = 100;
	this->pos = 50;
}

//Public methods
void Slider::Event(UI::Event& event)
{
	switch (event.GetType())
	{
	case EventType::ValueChanged:
	{
		ValueChangedEvent& vce = static_cast<ValueChangedEvent&>(event);

		this->pos = vce.GetNewValue().u32;
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
void Slider::RealizeSelf()
{
	_stdxx_::SliderBackend* sliderBackend = this->_GetUIBackend()->CreateSliderBackend(this);
	this->_SetBackend(sliderBackend);
}

//Event handlers
void Slider::OnRealized()
{
	Widget::OnRealized();

	this->sliderBackend->SetRange(this->min, this->max);
	this->sliderBackend->SetPosition(this->pos);
}
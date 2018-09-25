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
#include <Std++/UI/Controls/PushButton.hpp>
//Local
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/_Backends/UI/PushButtonBackend.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
PushButton::PushButton() : pushButtonBackend(nullptr)
{
	this->sizingPolicy.SetHorizontalPolicy(SizingPolicy::Policy::Minimum);
	this->sizingPolicy.SetVerticalPolicy(SizingPolicy::Policy::Fixed);
}

//Private methods
void PushButton::RealizeSelf()
{
	_stdxx_::PushButtonBackend* pushButtonBackend = this->_GetUIBackend()->CreatePushButtonBackend(this);
	this->_SetBackend(pushButtonBackend);
}

//Event handlers
void PushButton::OnRealized()
{
	Widget::OnRealized();
	this->pushButtonBackend->SetText(this->text);
}
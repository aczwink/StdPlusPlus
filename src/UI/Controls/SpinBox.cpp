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
#include <Std++/Integer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
SpinBox::SpinBox() : min(Integer<int32>::Min()), max(Integer<int32>::Max())
{
	this->sizingPolicy.SetHorizontalPolicy(SizingPolicy::Policy::Minimum);
	this->sizingPolicy.SetVerticalPolicy(SizingPolicy::Policy::Fixed);

	this->spinBoxBackend = BackendManager<UIBackend>::GetRootInstance().GetActiveBackend()->CreateSpinBoxBackend(this);
	this->backend = this->spinBoxBackend;

	this->spinBoxBackend->SetRange(this->min, this->max);
}
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
#include <Std++/UI/Containers/GroupBox.hpp>
//Local
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/UI/Layouts/GridLayout.hpp>
#include <Std++/UI/Containers/CompositeWidget.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
GroupBox::GroupBox() : ContentAreaWidget(),
	groupBoxBackend(nullptr)
{
	this->SetContentContainer(new CompositeWidget);
	this->GetContentContainer()->SetLayout(new VerticalLayout);
}

//Private methods
void GroupBox::RealizeSelf()
{
	_stdxx_::GroupBoxBackend* groupBoxBackend = this->_GetUIBackend()->CreateGroupBoxBackend(this);
	this->_SetBackend(groupBoxBackend);
}

//Event handlers
void GroupBox::OnRealized()
{
	ContentAreaWidget::OnRealized();

	this->groupBoxBackend->SetTitle(this->title);
}
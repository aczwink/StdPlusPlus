/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Views/View.hpp>
//Local
#include <Std++/UI/Controllers/TreeController.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Public methods
void View::Event(UI::Event& event)
{
	switch (event.GetType())
	{
	case EventType::SelectionChanged:
		this->OnSelectionChanged(static_cast<SelectionChangedEvent&>(event));
		event.Accept();
		break;
	default:
		Widget::Event(event);
	}
}

void View::Select(const ControllerIndex& index)
{
	this->selectionController.Select(index);
	if(this->viewBackend)
		this->viewBackend->UpdateSelection(); //inform ui

	this->selectionChanged.Emit();
}

void View::SetController(SharedPointer<TreeController> controller)
{
	this->controller = StdXX::Move(controller);
	this->controller->view = this;

	this->OnModelChanged();	
	this->controller->OnViewChanged();
}

//Eventhandlers
void View::OnModelChanged()
{
	if (this->viewBackend)
		this->viewBackend->ControllerChanged();
}

void View::OnRealized()
{
	Widget::OnRealized();

	if (!this->controller.IsNull())
		this->OnModelChanged();
}

void View::OnSelectionChanged(SelectionChangedEvent& event)
{
	if (event.DescribesDifference())
	{
		NOT_IMPLEMENTED_ERROR;
	}
	else
		this->selectionController.SetSelection(event.Selected());
	
	this->selectionChanged.Emit();
}
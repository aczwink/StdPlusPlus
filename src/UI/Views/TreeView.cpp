/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Views/TreeView.hpp>
//Local
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/UI/Containers/CompositeWidget.hpp>
#include <Std++/UI/Controllers/TreeController.hpp>
#include <Std++/UI/Layouts/GridLayout.hpp>
#include "TreeViewBody.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
TreeView::TreeView()
{
    this->sizingPolicy.SetHorizontalPolicy(SizingPolicy::Policy::Expanding);
    this->sizingPolicy.SetVerticalPolicy(SizingPolicy::Policy::Expanding);

	this->styleContext.AddType(u8"TreeView");

	this->headerView = nullptr;
	this->body = nullptr;
}

//Private methods
void TreeView::RealizeSelf()
{
	_stdxx_::ViewBackend* viewBackend = this->_GetUIBackend()->CreateTreeViewBackend(*this);
	if (viewBackend)
		this->_SetBackend(viewBackend);
	else
	{
		this->SetLayout(new VerticalLayout);

		this->headerView = new HeaderView;
		this->AddChild(this->headerView);
		this->body = new _stdxx_::TreeViewBody(*this->headerView, this->SelectionController());
		this->AddChild(this->body);
	}
}

//Event handlers
void TreeView::OnModelChanged()
{
	if (this->headerView)
	{
		if(this->headerView->GetController() != this->controller)
			this->headerView->SetController(this->controller);
		this->body->SetController(this->controller);
	}
	else
		View::OnModelChanged();
}
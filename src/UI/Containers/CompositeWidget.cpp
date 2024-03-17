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
#include <Std++/UI/Containers/CompositeWidget.hpp>
//Local
#include <Std++/UI/Windows/Window.hpp>
#include <Std++/UI/Layouts/GridLayout.hpp>
#include <Std++/UI/Containers/TabContainer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
CompositeWidget::CompositeWidget()
{
    this->layout = new GridLayout;
}

//Destructor
CompositeWidget::~CompositeWidget()
{
    while(!this->children.IsEmpty())
        //don't use PopFront, because child will delete itself from the children list
        //this way the child will find itself at the head of the list and removing will be fast
        //if we remove the child before, the child will loop through the whole list in order to (vainly) find itself
        delete this->children.GetFront();

    if(this->layout)
        delete this->layout;
}

//Eventhandlers
void CompositeWidget::OnPaint()
{
	if(this->_GetBackend() == nullptr)
	{
		for(Widget *const& refpChild : this->children)
			refpChild->Repaint();
	}
}

void CompositeWidget::OnResized()
{
    this->layout->Layout(*this);
}

void CompositeWidget::SetLayout(ILayout *pLayout)
{
    if(this->layout)
        delete this->layout;

    this->layout = pLayout;
}

//Public methods
Widget *CompositeWidget::GetChild(uint32 index)
{
	return this->children[index];
}

const Widget* CompositeWidget::GetChild(uint32 index) const
{
	return this->children[index];
}

uint32 CompositeWidget::GetNumberOfChildren() const
{
	return this->children.GetNumberOfElements();
}

Math::SizeD CompositeWidget::GetSizeHint() const
{
	Math::SizeD size;
	if(this->_GetBackend())
		size = size.Max(this->_GetBackend()->GetSizeHint());
    if(this->layout)
		size = size.Max(this->layout->GetPreferredSize(*this));

	return size;
}

//Private methods
void CompositeWidget::RealizeSelf()
{
	WidgetContainer* parent = this->GetParent();

	ContentAreaWidget* parentCAW = dynamic_cast<ContentAreaWidget *>(parent);
	if (parentCAW)
	{
		_stdxx_::WidgetContainerBackend *backend = parentCAW->_GetContentAreaWidgetBackend()->CreateContentAreaBackend(*this);
		this->_SetBackend(backend);
	}
	_stdxx_::TabContainerBackend *parentTabBackend = dynamic_cast<_stdxx_::TabContainerBackend *>(parent->_GetBackend());
	if(parentTabBackend)
	{
		_stdxx_::WidgetContainerBackend* backend = parentTabBackend->CreateChildAreaBackend(*this);
		this->_SetBackend(backend);
	}
}
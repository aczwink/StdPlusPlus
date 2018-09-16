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
#include <Std++/UI/Widget.hpp>
//Local
#include <Std++/UI/Containers/CompositeWidget.hpp>
#include <Std++/UI/Window.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Destructor
Widget::~Widget()
{
    if(this->parent)
        this->parent->RemoveChild(this);

    if(this->backend)
        delete this->backend;
}

//Public methods
Math::SizeD Widget::GetSizeHint() const
{
	if(this->backend)
		return this->backend->GetSizeHint();

	return Math::SizeD();
}

Window *Widget::GetWindow()
{
	if (IS_INSTANCE_OF(this, Window))
		return dynamic_cast<Window *>(this);
	if (this->parent)
		return this->parent->GetWindow();
	return nullptr;
}

const Window *Widget::GetWindow() const
{
	if (IS_INSTANCE_OF(this, const Window))
		return dynamic_cast<const Window *>(this);
	if (this->parent)
		return this->parent->GetWindow();
	return nullptr;
}

Math::PointD Widget::TranslateToAncestorCoords(const Math::PointD &point, const WidgetContainer *ancestor) const
{
	Math::PointD translated = point;
	const Widget *current = this;
	while(current != ancestor)
	{
		translated = current->TranslateToParentCoords(translated);
		current = current->parent;
	}
	return translated;
}

//Protected methods
UIBackend* Widget::_GetUIBackend()
{
	if (this->backend)
		return this->backend->GetUIBackend();
	return this->parent->_GetUIBackend();
}

//Private methods
bool Widget::CanRealize() const
{
	if (this->parent)
	{
		if (this->parent->isRealized)
			return true;
		return this->parent->CanRealize();
	}
	return false;
}

void Widget::Realize()
{
	//already realized?
	if (this->isRealized)
		return;
	this->isRealized = true; //do this early to prevent double realization by parents that realize their children

	//parent should also be realized
	if(this->parent)
		this->parent->Realize();

	//realize self
	this->RealizeSelf();
	this->OnRealized();
}

void Widget::RealizeSelf()
{
}

//Eventhandlers
void Widget::OnMouseButtonPressed(const Events::MouseClickEvent &event)
{
	this->IgnoreEvent();
}

void Widget::OnMouseButtonReleased(const Events::MouseClickEvent &event)
{
	this->IgnoreEvent();
}

void Widget::OnMouseMoved(const Math::PointD &pos)
{
	this->IgnoreEvent();
}

void Widget::OnMouseWheelTurned(int16 delta)
{
	this->IgnoreEvent();
}

void Widget::OnMoved()
{
	this->IgnoreEvent();
}

void Widget::OnPaint()
{
	this->IgnoreEvent();
}

void Widget::OnRealized()
{
	this->IgnoreEvent();
}

void Widget::OnResized()
{
    this->IgnoreEvent();
}
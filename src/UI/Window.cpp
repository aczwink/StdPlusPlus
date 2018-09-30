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
#include <Std++/UI/Window.hpp>
//Local
#include <Std++/_Backends/UI/WindowBackend.hpp>
#include <Std++/UI/Menu/MenuBar.hpp>
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/UI/UIBackend.hpp>
#include "Render Targets/IRenderTarget.h"
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructors
Window::Window() : canRealize(false), windowBackend(nullptr)
{
    this->pMenuBar = nullptr;
    this->pOSDropTarget = nullptr;
	
	this->SetContentContainer(new CompositeWidget);

	this->Show(false);
	this->canRealize = true;
}

//Destructor
Window::~Window()
{
    if(this->pMenuBar)
        delete this->pMenuBar;
    if(this->pOSDropTarget)
        delete this->pOSDropTarget;
}

//Eventhandlers
void Window::OnClose()
{
    delete this;
}

EDropType Window::OnDragEnter(const ITransfer &refTransfer)
{
    return EDropType::None;
}

void Window::OnDragLeave()
{
}

EDropType Window::OnDragMove()
{
    return EDropType::None;
}

void Window::OnDrop(const ITransfer &refTransfer)
{
}

void Window::OnRealized()
{
	WidgetContainer::OnRealized();

	this->windowBackend->SetTitle(this->title);
}

//Public methods
void Window::Event(UI::Event& e)
{
	switch (e.GetType())
	{
	case EventType::WindowShouldBeClosed:
		this->OnClose();
		e.Accept();
		break;
	default:
		ContentAreaWidget::Event(e);
	}
}

void Window::SwitchFullscreen(bool state)
{
    if(this->_GetBackend())
    {
        //OS-handled!
        NOT_IMPLEMENTED_ERROR;
    }
    else
    {
		//this->SetBounds(Rect(Point(), GetRenderTarget(this->renderMode).GetSize()));
        NOT_IMPLEMENTED_ERROR;
        this->Repaint();
    }
}

//Private methods
bool Window::CanRealize() const
{
	return this->canRealize;
}

void Window::RealizeSelf()
{
	_stdxx_::WindowBackend* windowBackend = BackendManager<UIBackend>::GetRootInstance().GetActiveBackend()->CreateWindowBackend(this);
	this->_SetBackend(windowBackend);
}
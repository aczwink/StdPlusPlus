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
#include <Std++/UI/Menu/CMenuBar.h>
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/UIBackend.hpp>
#include "Render Targets/IRenderTarget.h"
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Constructors
Window::Window(ERenderMode mode) : WidgetContainer(nullptr)
{
    this->pMenuBar = nullptr;
    this->pOSDropTarget = nullptr;
    this->renderMode = mode;

	this->backend = BackendManager<UIBackend>::GetRootInstance().GetActiveBackend()->CreateWindowBackend(_stdpp::WindowBackendType::Window, this);

	//position and size
    const float64 shrinkPercentage = -0.1;

    Size screenSize = GetRenderTarget(mode).GetSize();

	Rect windowRect;
    windowRect.width() = screenSize.width;
    windowRect.height() = screenSize.height;
    windowRect.Enlarge(int32(windowRect.width() * shrinkPercentage), int32(windowRect.height() * shrinkPercentage));

	this->SetBounds(windowRect);
}

Window::Window(const Rect &refRect, ERenderMode mode) : WidgetContainer(nullptr)
{
    this->pMenuBar = nullptr;
    this->pOSDropTarget = nullptr;
    this->renderMode = mode;

	this->backend = BackendManager<UIBackend>::GetRootInstance().GetActiveBackend()->CreateWindowBackend(_stdpp::WindowBackendType::Window, this);
	this->SetBounds(refRect);
}

Window::Window(uint16 width, uint16 height, ERenderMode mode) : WidgetContainer(nullptr)
{
    this->pMenuBar = nullptr;
    this->pOSDropTarget = nullptr;
    this->renderMode = mode;

	this->backend = BackendManager<UIBackend>::GetRootInstance().GetActiveBackend()->CreateWindowBackend(_stdpp::WindowBackendType::Window, this);

	//position and size
    Size screenSize = GetRenderTarget(mode).GetSize();

	Rect windowRect;
    windowRect.x() = (screenSize.width - width) / 2;
    windowRect.y() = (screenSize.height - height) / 2;
    windowRect.width() = width;
    windowRect.height() = height;

	this->SetBounds(windowRect);
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

//Public methods
void Window::SetMenuBar(CMenuBar *pMenuBar)
{
    if(this->pMenuBar)
        delete this->pMenuBar;

    this->pMenuBar = pMenuBar;

    this->MenuBarChangeOS();
}

void Window::SwitchFullscreen(bool state)
{
    if(this->backend)
    {
        //OS-handled!
        NOT_IMPLEMENTED_ERROR;
    }
    else
    {
		this->SetBounds(Rect(Point(), GetRenderTarget(this->renderMode).GetSize()));
        this->Repaint();
    }
}
/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACStdLib/UI/Window.h>
//Local
#include <ACStdLib/UI/Menu/CMenuBar.h>
#include "Render Targets/IRenderTarget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Constructors
Window::Window(ERenderMode mode) : AWidgetContainer(nullptr)
{
    CSize screenSize;
    CRect windowRect;

    const float64 shrinkPercentage = -0.1;

    this->pMenuBar = nullptr;
    this->pOSDropTarget = nullptr;
    this->renderMode = mode;

    screenSize = GetRenderTarget(mode).GetSize();

    windowRect.width() = screenSize.width;
    windowRect.height() = screenSize.height;
    windowRect.Enlarge(int32(windowRect.width() * shrinkPercentage), int32(windowRect.height() * shrinkPercentage));

    this->Init(windowRect);
}

Window::Window(const CRect &refRect, ERenderMode mode) : AWidgetContainer(nullptr)
{
    this->pMenuBar = nullptr;
    this->pOSDropTarget = nullptr;
    this->renderMode = mode;

    this->Init(refRect);
}

Window::Window(uint16 width, uint16 height, ERenderMode mode) : AWidgetContainer(nullptr)
{
    CRect windowRect;
    CSize screenSize;

    this->pMenuBar = nullptr;
    this->pOSDropTarget = nullptr;
    this->renderMode = mode;

    screenSize = GetRenderTarget(mode).GetSize();

    windowRect.x() = (screenSize.width - width) / 2;
    windowRect.y() = (screenSize.height - height) / 2;
    windowRect.width() = width;
    windowRect.height() = height;

    this->Init(windowRect);
}

//Destructor
Window::~Window()
{
    if(this->pMenuBar)
        delete this->pMenuBar;
    if(this->pOSDropTarget)
        delete this->pOSDropTarget;

    this->DestroyOSWindow();
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

//Private methods
void Window::Init(const CRect &refRect)
{
    switch(this->renderMode)
    {
        case ERenderMode::OS:
            this->CreateOSWindow(refRect);
            break;
        case ERenderMode::Text:
            this->SetRect(refRect);
            break;
    }
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
    if(this->pOSHandle)
    {
        //OS-handled!
        NOT_IMPLEMENTED_ERROR;
    }
    else
    {
        this->SetRect(CRect(CPoint(), GetRenderTarget(this->renderMode).GetSize()));
        this->Repaint();
    }
}
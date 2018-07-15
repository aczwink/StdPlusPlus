/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "CocoaBackend.hh"
//Local
#include "UI/CocoaEventSource.hh"
#include "UI/CocoaWindowBackend.hh"
#include "UI/CocoaMenuBarBackend.hh"
//Namespaces
using namespace _stdpp;
using namespace StdPlusPlus::UI;
//Cocoa
#undef new
#include <Cocoa/Cocoa.h>

//Public methods
MenuBackend *StdPlusPlus::CocoaBackend::CreateMenuBackend(StdPlusPlus::UI::Menu *menu)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

MenuBarBackend *StdPlusPlus::CocoaBackend::CreateMenuBarBackend(StdPlusPlus::UI::MenuBar *menuBar)
{
	return new CocoaMenuBarBackend();
}

WindowBackend *StdPlusPlus::CocoaBackend::CreateWindowBackend(WindowBackendType type, StdPlusPlus::UI::Widget *widget)
{
	return new CocoaWindowBackend(this, type, widget);
}

StdPlusPlus::EventSource *StdPlusPlus::CocoaBackend::GetEventSource()
{
	return this->eventSource;
}

void StdPlusPlus::CocoaBackend::Load()
{
	this->eventSource = new CocoaEventSource;
	[NSApplication sharedApplication];
}

void StdPlusPlus::CocoaBackend::Unload() const
{
	[NSApp release];
	delete this->eventSource;
}
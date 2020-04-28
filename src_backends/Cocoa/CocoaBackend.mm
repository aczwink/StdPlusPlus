/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Displays/RenderTargetWidget.hpp>
#include <Std++/UI/Window.hpp>
#import "UI/CocoaCheckBoxBackend.hh"
#include "UI/CocoaEventSource.hh"
#include "UI/CocoaWindowBackend.hh"
#include "UI/CocoaMenuBarBackend.hh"
#import "UI/CocoaRenderTargetWidgetBackend.hh"
#import "UI/CocoaGroupBoxBackend.hh"
#import "UI/CocoaPushButtonBackend.hh"
#import "UI/CocoaLabelBackend.hh"
#import "UI/CocoaScrollAreaBackend.hh"
#import "UI/CocoaSliderBackend.hh"
#import "UI/CocoaSpinBoxBackend.hh"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;
//Cocoa
#undef new
#include <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

NSAutoreleasePool *g_autoreleasePool;

//Public methods
CheckBoxBackend *StdXX::CocoaBackend::CreateCheckBoxBackend(CheckBox *checkBox)
{
	return new CocoaCheckBoxBackend(*this, checkBox);
}

DrawableWidgetBackend *CocoaBackend::CreateDrawableWidgetBackend(UI::Widget &widget)
{
	return nullptr;
}

StdXX::EventSource *StdXX::CocoaBackend::CreateEventSource()
{
	return new CocoaEventSource;
}

GroupBoxBackend *CocoaBackend::CreateGroupBoxBackend(GroupBox& groupBox)
{
	return new CocoaGroupBoxBackend(*this, &groupBox);
}

HeaderViewBackend *CocoaBackend::CreateHeaderViewBackend(UI::HeaderView &headerView)
{
	return nullptr;
}

LabelBackend *CocoaBackend::CreateLabelBackend(UI::Label *label)
{
	return new CocoaLabelBackend(*this, label);
}

MenuBackend *StdXX::CocoaBackend::CreateMenuBackend(StdXX::UI::Menu *menu)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

MenuBarBackend *StdXX::CocoaBackend::CreateMenuBarBackend(StdXX::UI::MenuBar *menuBar)
{
	return new CocoaMenuBarBackend();
}

PushButtonBackend *CocoaBackend::CreatePushButtonBackend(UI::PushButton& pushButton)
{
	return new CocoaPushButtonBackend(*this, &pushButton);
}

WidgetBackend *CocoaBackend::CreateRenderTargetWidgetBackend(RenderTargetWidget& renderTargetWidget)
{
	return new CocoaRenderTargetWidgetBackend(*this, &renderTargetWidget);
}

ContentAreaWidgetBackend *CocoaBackend::CreateScrollAreaBackend(UI::ScrollArea *scrollArea)
{
	return new CocoaScrollAreaBackend(*this, *scrollArea);
}

WidgetBackend *CocoaBackend::CreateSearchBoxBackend(UI::SearchBox &searchBox)
{
	return nullptr;
}

ViewBackend *CocoaBackend::CreateSelectBoxBackend(UI::SelectBox &selectBox)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

SliderBackend *CocoaBackend::CreateSliderBackend(UI::Slider& slider)
{
	return new CocoaSliderBackend(*this, &slider);
}

SpinBoxBackend *CocoaBackend::CreateSpinBoxBackend(UI::SpinBox *spinBox)
{
	return new CocoaSpinBoxBackend(*this, spinBox);
}

ViewBackend *CocoaBackend::CreateTableViewBackend(UI::TableView &treeView)
{
	return nullptr;
}

ViewBackend *CocoaBackend::CreateTreeViewBackend(UI::TreeView &treeView)
{
	return nullptr;
}

WindowBackend *StdXX::CocoaBackend::CreateWindowBackend(Window *window)
{
	return new CocoaWindowBackend(*this, window);
}

void StdXX::CocoaBackend::Load()
{
	g_autoreleasePool = [[NSAutoreleasePool alloc] init];
	[NSApplication sharedApplication];
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
}

void StdXX::CocoaBackend::Unload() const
{
	[g_autoreleasePool drain];
	[NSApp release];
}
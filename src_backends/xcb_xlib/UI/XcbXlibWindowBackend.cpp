/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
#include "XcbXlibWindowBackend.hpp"
//Local
#include <Std++/UI/Window.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//X layer
#include "../XLayer/XConnection.hpp"
#include "../XLayer/XcbWindow.hpp"

//Constructor
XcbXlibWindowBackend::XcbXlibWindowBackend(UIBackend& uiBackend, XConnection& xConnection, UI::Window& window)
	: WindowBackend(uiBackend), WidgetContainerBackend(uiBackend), WidgetBackend(uiBackend),
	xConnection(xConnection), window(window)
{
	this->xcbWindow = new XcbWindow(xConnection, xConnection.GetDefaultScreen()->root_visual);
}

//Destructor
XcbXlibWindowBackend::~XcbXlibWindowBackend()
{
}

//Public methods
void XcbXlibWindowBackend::AddChild(UI::Widget *widget)
{
}

WidgetContainerBackend* XcbXlibWindowBackend::CreateContentAreaBackend(UI::CompositeWidget &widget)
{
	return nullptr;
}

const StdXX::UI::Widget &XcbXlibWindowBackend::GetWidget() const
{
	return this->window;
}

void XcbXlibWindowBackend::Maximize()
{
	this->xcbWindow->SetGeometry({0, 0, this->xConnection.GetDefaultScreen()->width_in_pixels, this->xConnection.GetDefaultScreen()->height_in_pixels});
}

void XcbXlibWindowBackend::SetTitle(const String &title)
{
	this->xcbWindow->SetTitle(title);
}

void XcbXlibWindowBackend::Show(bool visible)
{
	this->xcbWindow->Map(visible);
}



//NOT IMPLEMENTED
StdXX::Math::RectD _stdxx_::XcbXlibWindowBackend::GetContentAreaBounds() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Math::RectD();
}

StdXX::Math::SizeD _stdxx_::XcbXlibWindowBackend::GetSizeHint() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Math::SizeD();
}

void _stdxx_::XcbXlibWindowBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::XcbXlibWindowBackend::SetBounds(const StdXX::Math::RectD &bounds)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::XcbXlibWindowBackend::SetEnabled(bool enable)
{
	ASSERT(enable, u8"IMPLEMENT THIS"); //TODO: implement me
}

void _stdxx_::XcbXlibWindowBackend::SetHint(const StdXX::String &text)
{
	ASSERT(text.IsEmpty(), u8"IMPLEMENT THIS"); //TODO: implement me
}

StdXX::FileSystem::Path _stdxx_::XcbXlibWindowBackend::SelectExistingDirectory(const StdXX::String &title,
                                                                   const StdXX::Function<bool(
		                                                                   StdXX::FileSystem::Path &)> callback) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::FileSystem::Path();
}

StdXX::FileSystem::Path _stdxx_::XcbXlibWindowBackend::SelectExistingFile(const StdXX::String &title,
                                                              const StdXX::DynamicArray<StdXX::Tuple<StdXX::String, StdXX::DynamicArray<StdXX::String>>> &filters,
                                                              const StdXX::FileSystem::Path &initialPath) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::FileSystem::Path();
}

void _stdxx_::XcbXlibWindowBackend::SetMenuBar(StdXX::UI::MenuBar *menuBar, _stdxx_::MenuBarBackend *menuBarBackend)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::XcbXlibWindowBackend::ShowErrorBox(const StdXX::String &title, const StdXX::String &message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::XcbXlibWindowBackend::ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

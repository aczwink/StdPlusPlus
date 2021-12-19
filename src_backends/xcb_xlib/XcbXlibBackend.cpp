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
#include "XcbXlibBackend.hpp"
//Local
#include "UI/XcbXlibEventSource.hpp"
#include "UI/XcbXlibWindowBackend.hpp"
#include "XLayer/XConnection.hpp"
#include "UI/XcbXlibRenderTargetWidgetBackend.hpp"
#include "XcbXlibOpenGLBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
XcbXlibBackend::XcbXlibBackend()
{
	this->xConnection = new XConnection;

	//register render backends
#ifdef _STDPLUSPLUS_BACKEND_OPENGL
	XcbXlibOpenGLBackend *openGLBackend = new XcbXlibOpenGLBackend;
    this->renderBackends.RegisterBackend(openGLBackend, 0);
#endif
}

//Destructor
XcbXlibBackend::~XcbXlibBackend()
{
	delete this->xConnection;
}

//Public methods
_stdxx_::CheckBoxBackend *_stdxx_::XcbXlibBackend::CreateCheckBoxBackend(StdXX::UI::CheckBox& checkBox)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

ClipboardBackend *XcbXlibBackend::CreateClipboardBackend() {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

_stdxx_::DrawableWidgetBackend *_stdxx_::XcbXlibBackend::CreateDrawableWidgetBackend(StdXX::UI::Widget &widget)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

EventHandling::EventSource* XcbXlibBackend::CreateEventSource()
{
	return new XcbXlibEventSource(*this->xConnection);
}

_stdxx_::GroupBoxBackend *_stdxx_::XcbXlibBackend::CreateGroupBoxBackend(StdXX::UI::GroupBox& groupBox)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

_stdxx_::HeaderViewBackend *_stdxx_::XcbXlibBackend::CreateHeaderViewBackend(StdXX::UI::HeaderView &headerView)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

_stdxx_::LabelBackend *_stdxx_::XcbXlibBackend::CreateLabelBackend(StdXX::UI::Label& label)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

ViewBackend *XcbXlibBackend::CreateListViewBackend(UI::ListView &listView)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

_stdxx_::MenuBackend *_stdxx_::XcbXlibBackend::CreateMenuBackend(StdXX::UI::Menu& menu)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

MenuBarBackend *XcbXlibBackend::CreateMenuBarBackend(UI::MenuBar *menuBar)
{
	return nullptr;
}

ProgressBarBackend *XcbXlibBackend::CreateProgressBarBackend(UI::ProgressBar &progressBar)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

_stdxx_::PushButtonBackend *_stdxx_::XcbXlibBackend::CreatePushButtonBackend(StdXX::UI::PushButton& pushButton)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

WidgetBackend *XcbXlibBackend::CreateRenderTargetWidgetBackend(UI::RenderTargetWidget& renderTargetWidget)
{
	return new XcbXlibRenderTargetWidgetBackend(*this, *this->xConnection, renderTargetWidget);
}

_stdxx_::ContentAreaWidgetBackend *_stdxx_::XcbXlibBackend::CreateScrollAreaBackend(StdXX::UI::ScrollArea *scrollArea)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

_stdxx_::WidgetBackend *_stdxx_::XcbXlibBackend::CreateSearchBoxBackend(StdXX::UI::SearchBox &searchBox)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

_stdxx_::ViewBackend *_stdxx_::XcbXlibBackend::CreateSelectBoxBackend(StdXX::UI::SelectBox &selectBox)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

_stdxx_::SliderBackend *_stdxx_::XcbXlibBackend::CreateSliderBackend(StdXX::UI::Slider& slider)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

_stdxx_::SpinBoxBackend *_stdxx_::XcbXlibBackend::CreateSpinBoxBackend(StdXX::UI::SpinBox *spinBox)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

TabContainerBackend *XcbXlibBackend::CreateTabContainerBackend(UI::TabContainer &tabContainer)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

_stdxx_::ViewBackend *_stdxx_::XcbXlibBackend::CreateTableViewBackend(StdXX::UI::TableView &treeView)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

TextEditBackend *XcbXlibBackend::CreateTextEditBackend(UI::TextEdit &textEdit) {
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}


_stdxx_::ViewBackend *_stdxx_::XcbXlibBackend::CreateTreeViewBackend(StdXX::UI::TreeView &treeView)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

WindowBackend *XcbXlibBackend::CreateWindowBackend(UI::Window *window)
{
	return new XcbXlibWindowBackend(*this, *this->xConnection, *window);
}
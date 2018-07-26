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
#include "CocoaCompositeView.hh"
//Local
#include <Std++/UI/Layouts/ILayout.h>
#import "CocoaEventSource.hh"
#import "CocoaSpinBoxBackend.hh"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Destructor
CocoaCompositeView::~CocoaCompositeView()
{
	this->backend = nullptr; //or the Widget destructor will destroy us again
}

//Public methods
void CocoaCompositeView::AddChild(StdXX::UI::Widget *widget)
{
	CocoaSpinBoxBackend *spinBoxBackend = dynamic_cast<CocoaSpinBoxBackend *>(widget->_GetBackend());
	if(spinBoxBackend)
	{
		[this->view addSubview:spinBoxBackend->GetTextField()];
		[this->view addSubview:spinBoxBackend->GetStepper()];
		return;
	}

	CocoaView *cocoaView = dynamic_cast<CocoaView *>(widget->_GetBackend());
	[this->view addSubview:cocoaView->GetView()];
}

StdXX::Math::SizeD _stdxx_::CocoaCompositeView::GetSizeHint() const
{
	if(this->layout)
		return this->layout->GetPreferredSize(*this);
	return Math::SizeD();
}

UI::Widget &CocoaCompositeView::GetWidget()
{
	return *this;
}

void CocoaCompositeView::SetBounds(const StdXX::Math::RectD &area)
{
	CocoaEventSource::EmitResizingEvent(*this, area);
	[this->view setFrame:NSMakeRect(area.origin.x, area.origin.y, area.width(), area.height())];
	CocoaEventSource::EmitResizedEvent(*this);
}








//OLD STUFF
_stdxx_::WidgetBackend *_stdxx_::CocoaCompositeView::CreateChildBackend(StdXX::UI::Widget *widget) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return nullptr;
}

StdXX::Math::SizeD _stdxx_::CocoaCompositeView::GetSize() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Math::SizeD();
}

void _stdxx_::CocoaCompositeView::Paint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaCompositeView::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaCompositeView::Select(StdXX::UI::ControllerIndex &controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

StdXX::Path _stdxx_::CocoaCompositeView::SelectExistingDirectory(const StdXX::String &title,
														const StdXX::Function<bool(StdXX::Path &)> callback) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::Path();
}

void _stdxx_::CocoaCompositeView::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaCompositeView::SetEnabled(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaCompositeView::SetHint(const StdXX::String &text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaCompositeView::Show(bool visible)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaCompositeView::ShowInformationBox(const StdXX::String &title, const StdXX::String &message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaCompositeView::UpdateSelection(StdXX::UI::SelectionController &selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaCompositeView::IgnoreEvent()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 _stdxx_::CocoaCompositeView::GetPosition() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}

void _stdxx_::CocoaCompositeView::GetRange(int32 &min, int32 &max)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaCompositeView::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaCompositeView::SetMenuBar(StdXX::UI::MenuBar *menuBar, _stdxx_::MenuBarBackend *menuBarBackend)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CocoaCompositeView::RemoveChild(StdXX::UI::Widget *child)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
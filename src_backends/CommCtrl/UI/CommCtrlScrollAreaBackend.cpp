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
#include "CommCtrlScrollAreaBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
WidgetContainerBackend* CommCtrlScrollAreaBackend::CreateContentAreaBackend(CompositeWidget & widget)
{
	return nullptr;
}

RectD CommCtrlScrollAreaBackend::GetContentAreaBounds() const
{
	return RectD(PointD(), this->scrollArea->GetSizeHint());
}

SizeD CommCtrlScrollAreaBackend::GetSizeHint() const
{
	//TODO: actually sizes of both scroll bars...
	return SizeD(); //no requirements
}

void CommCtrlScrollAreaBackend::SetBounds(const RectD &bounds)
{
	CommCtrlWidgetBackend::SetBounds(bounds);

	RectD contentBounds = this->GetContentAreaBounds();
	
	ShowScrollBar(this->GetHWND(), SB_HORZ, contentBounds.width() > bounds.width());
	ShowScrollBar(this->GetHWND(), SB_VERT, contentBounds.height() > bounds.height());
}






















//Not implemented:
void _stdxx_::CommCtrlScrollAreaBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::SetEnabled(bool enable)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::SetHint(const StdXX::String & text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

uint32 _stdxx_::CommCtrlScrollAreaBackend::GetPosition() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
	return uint32();
}

void _stdxx_::CommCtrlScrollAreaBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::Select(StdXX::UI::ControllerIndex & controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::UpdateSelection(StdXX::UI::SelectionController & selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::SetMenuBar(StdXX::UI::MenuBar * menuBar, MenuBarBackend * menuBarBackend)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}

void _stdxx_::CommCtrlScrollAreaBackend::ShowInformationBox(const StdXX::String & title, const StdXX::String & message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implementme
}
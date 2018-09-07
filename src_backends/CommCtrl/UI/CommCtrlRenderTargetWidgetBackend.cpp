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
#include "CommCtrlRenderTargetWidgetBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
SizeD CommCtrlRenderTargetWidgetBackend::GetSizeHint() const
{
	return SizeD();
}

Widget & CommCtrlRenderTargetWidgetBackend::GetWidget()
{
	return *this->renderTargetWidget;
}

const StdXX::UI::Widget & _stdxx_::CommCtrlRenderTargetWidgetBackend::GetWidget() const
{
	return *this->renderTargetWidget;
}

void CommCtrlRenderTargetWidgetBackend::PrePaint()
{
	RECT rcUpdate;
	
	//validate the update region (it is important that this is done before calling the event handler because user might want to redraw in paint handler!!!)
	GetUpdateRect(this->GetHWND(), &rcUpdate, FALSE);
	ValidateRect(this->GetHWND(), &rcUpdate);
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::Select(StdXX::UI::ControllerIndex & controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::SetHint(const StdXX::String & text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::Show(bool visible)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::ShowInformationBox(const StdXX::String & title, const StdXX::String & message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::UpdateSelection(StdXX::UI::SelectionController & selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 _stdxx_::CommCtrlRenderTargetWidgetBackend::GetPosition() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return uint32();
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlRenderTargetWidgetBackend::SetMenuBar(StdXX::UI::MenuBar * menuBar, MenuBarBackend * menuBarBackend)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

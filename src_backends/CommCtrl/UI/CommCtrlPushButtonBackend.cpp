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
#include "CommCtrlPushButtonBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
SizeD CommCtrlPushButtonBackend::GetSizeHint() const
{
	//TODO: calc min width
	//TODO: this seems to be working... dont known how it is with different fonts

	SIZE size{};
	this->SendMessage(BCM_GETIDEALSIZE, 0, (LPARAM)&size);
	if (size.cy < 25)
		size.cy = 25; //aesthetics
	return SizeD(size.cx, size.cy);
}

Widget &CommCtrlPushButtonBackend::GetWidget()
{
	return *this->pushButton;
}

const StdXX::UI::Widget & _stdxx_::CommCtrlPushButtonBackend::GetWidget() const
{
	return *this->pushButton;
}

void _stdxx_::CommCtrlPushButtonBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlPushButtonBackend::Select(StdXX::UI::ControllerIndex & controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlPushButtonBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlPushButtonBackend::SetHint(const StdXX::String & text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlPushButtonBackend::Show(bool visible)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlPushButtonBackend::ShowInformationBox(const StdXX::String & title, const StdXX::String & message) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlPushButtonBackend::UpdateSelection(StdXX::UI::SelectionController & selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 _stdxx_::CommCtrlPushButtonBackend::GetPosition() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return uint32();
}

void _stdxx_::CommCtrlPushButtonBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlPushButtonBackend::SetMenuBar(StdXX::UI::MenuBar * menuBar, MenuBarBackend * menuBarBackend)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlPushButtonBackend::SetText(const StdXX::String & text)
{
	Win32Window::SetText(text);
}
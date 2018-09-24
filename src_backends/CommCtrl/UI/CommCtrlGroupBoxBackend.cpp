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
#include "CommCtrlGroupBoxBackend.hpp"
//Local
#include <Std++/UI/Containers/CompositeWidget.hpp>
#include "CommCtrlContainerBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
WidgetContainerBackend * CommCtrlGroupBoxBackend::CreateContentAreaBackend(CompositeWidget & widget)
{
	return nullptr;
}

RectD CommCtrlGroupBoxBackend::GetContentAreaBounds() const
{
	RectD result;
	result.size = this->groupBox->GetSize();
	result.Enlarge(-5, -5);
	result.height() -= 10; //TODO... title of course dependant to font
	return result;
}

SizeD CommCtrlGroupBoxBackend::GetSizeHint() const
{
	return this->GetTextExtents().Cast<float64>();
}

StdXX::UI::Widget &_stdxx_::CommCtrlGroupBoxBackend::GetWidget()
{
	return *this->groupBox;
}

const StdXX::UI::Widget &_stdxx_::CommCtrlGroupBoxBackend::GetWidget() const
{
	return *this->groupBox;
}

void _stdxx_::CommCtrlGroupBoxBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlGroupBoxBackend::Reparent(Win32Window *newParent)
{
	this->SetParent(newParent);
	CommCtrlContainerBackend::Reparent(newParent);
}

void _stdxx_::CommCtrlGroupBoxBackend::Select(StdXX::UI::ControllerIndex &controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlGroupBoxBackend::SetBounds(const RectD &bounds)
{
	this->SetRect(this->ToWinAPIBounds(bounds));
}

void _stdxx_::CommCtrlGroupBoxBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlGroupBoxBackend::SetHint(const StdXX::String &text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlGroupBoxBackend::UpdateSelection(StdXX::UI::SelectionController &selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlGroupBoxBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void CommCtrlGroupBoxBackend::SetTitle(const String &title)
{
	this->SetText(title);
}
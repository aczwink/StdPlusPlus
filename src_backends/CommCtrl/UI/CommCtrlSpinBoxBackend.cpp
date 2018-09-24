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
#include "CommCtrlSpinBoxBackend.hpp"
//Local
#include <Std++/UI/Controls/SpinBox.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
SizeD CommCtrlSpinBoxBackend::GetSizeHint() const
{
	//TODO: min width
	//TODO: this seems to be working... dont known how it is with different fonts

	return SizeD(40, 26);
}

int32 CommCtrlSpinBoxBackend::GetValue() const
{
	BOOL valid;
	int32 value = this->upDownControl.SendMessage(UDM_GETPOS32, 0, (LPARAM)&valid);
	if (valid != 0)
		return 0;
	return value;
}

Widget &CommCtrlSpinBoxBackend::GetWidget()
{
	return *this->spinBox;
}

const Widget &CommCtrlSpinBoxBackend::GetWidget() const
{
	return *this->spinBox;
}

void CommCtrlSpinBoxBackend::Reparent(Win32Window *newParent)
{
	this->editControl.SetParent(newParent);
	this->upDownControl.SetParent(newParent);
}

void CommCtrlSpinBoxBackend::SetBounds(const RectD &bounds)
{
	int w2 = 20;
	int w1 = bounds.width() - w2;
	if (bounds.width() <= w2)
		w1 = w2 = bounds.width() / 2;

	RectD editBounds(bounds.x(), bounds.y(), w1, bounds.height());
	RectD upDownBounds(bounds.x() + w1, bounds.y(), w2, bounds.height());

	this->editControl.SetRect(this->ToWinAPIBounds(editBounds));
	this->upDownControl.SetRect(this->ToWinAPIBounds(upDownBounds));
}

void CommCtrlSpinBoxBackend::SetRange(int32 min, int32 max)
{
	this->upDownControl.SendMessage(UDM_SETRANGE32, min, max);
	//update pos always because of redraw
	this->SetValue(this->GetValue());
}

void CommCtrlSpinBoxBackend::SetValue(int32 value)
{
	this->upDownControl.SendMessage(UDM_SETPOS32, 0, value);
}

void CommCtrlSpinBoxBackend::Show(bool visible)
{
	this->editControl.Show(visible);
	this->upDownControl.Show(visible);
}





//not implemented
void _stdxx_::CommCtrlSpinBoxBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlSpinBoxBackend::Select(StdXX::UI::ControllerIndex & controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlSpinBoxBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlSpinBoxBackend::SetEnabled(bool enable)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlSpinBoxBackend::SetHint(const StdXX::String & text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlSpinBoxBackend::UpdateSelection(StdXX::UI::SelectionController & selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlSpinBoxBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
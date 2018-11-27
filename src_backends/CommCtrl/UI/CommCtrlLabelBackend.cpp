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
#include "CommCtrlLabelBackend.hpp"
//Local
#include <Std++/UI/Controls/Label.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
SizeD CommCtrlLabelBackend::GetSizeHint() const
{
	return this->GetTextExtents().Cast<float64>();
}

Widget &CommCtrlLabelBackend::GetWidget()
{
	return *this->label;
}

const Widget & CommCtrlLabelBackend::GetWidget() const
{
	return *this->label;
}

void CommCtrlLabelBackend::SetText(const String & text)
{
	Win32Window::SetText(text);
}



//not implemented:
void _stdxx_::CommCtrlLabelBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlLabelBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
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
#include "CommCtrlSearchBoxBackend.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Public methods
Math::SizeD CommCtrlSearchBoxBackend::GetSizeHint() const
{
	auto extent = this->GetTextExtents(u8"Search...").Cast<float64>();
	if (extent.height < 21)
		extent.height = 21;

	return extent;
}

Widget & CommCtrlSearchBoxBackend::GetWidget()
{
	return this->searchBox;
}

const Widget & CommCtrlSearchBoxBackend::GetWidget() const
{
	return this->searchBox;
}










//NOT IMPELEMENTED

void _stdxx_::CommCtrlSearchBoxBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

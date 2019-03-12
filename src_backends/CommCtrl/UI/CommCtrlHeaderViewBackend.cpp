/*
* Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include "CommCtrlHeaderViewBackend.hpp"
//Local
#include <Std++/UI/Views/HeaderView.hpp>
#include <Std++/UI/WidgetContainer.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX::Math;

//Public methods
SizeD CommCtrlHeaderViewBackend::GetSizeHint() const
{
	RectD parentRect = this->headerView.GetParent()->GetLocalBounds();
	RECT rc;
	rc.left = parentRect.x();
	rc.right = parentRect.GetHorizontalEnd();
	rc.top = parentRect.y();
	rc.bottom = parentRect.GetVerticalEnd();

	HDLAYOUT hdl;
	WINDOWPOS wp;
	hdl.prc = &rc;
	hdl.pwpos = &wp;

	this->SendMessage(HDM_LAYOUT, 0, (LPARAM)&hdl);

	return SizeD(wp.cx, wp.cy);
}

void CommCtrlHeaderViewBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR;
}
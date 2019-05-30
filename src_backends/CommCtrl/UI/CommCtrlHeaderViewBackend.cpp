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
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
void CommCtrlHeaderViewBackend::ControllerChanged()
{
	//clear view
	uint32 count = this->GetItemCount();
	for (uint32 i = count; i > 0; i--)
	{
		this->SendMessage(HDM_DELETEITEM, i - 1, 0);
	}

	//add items
	if (this->headerView.HasController())
	{
		const auto& controller = this->headerView.GetController();
		uint32 count = controller->GetNumberOfColumns();
		for (uint32 i = 0; i < count; i++)
		{
			HDITEMW hdi;
			
			hdi.mask = HDI_TEXT | HDI_FORMAT;
			hdi.fmt = HDF_LEFT | HDF_STRING;

			String columnText = controller->GetColumnText(i);

			hdi.pszText = (LPWSTR)columnText.ToUTF16().GetRawZeroTerminatedData();
			SendMessageW(this->GetHWND(), HDM_INSERTITEMW, (WPARAM)i, (LPARAM)&hdi);
		}
	}

	this->AutoSizeColumns();
}

RectD CommCtrlHeaderViewBackend::GetItemRect(uint32 number) const
{
	RECT rc;
	this->SendMessage(HDM_GETITEMRECT, number, (LPARAM)&rc);

	return {
		(float64)rc.left,
		this->headerView.GetSize().height - rc.bottom,
		float64(rc.right - rc.left),
		float64(rc.bottom - rc.top)
	};

	return StdXX::Math::RectD();
}

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

Widget & CommCtrlHeaderViewBackend::GetWidget()
{
	return this->headerView;
}

const Widget & CommCtrlHeaderViewBackend::GetWidget() const
{
	return this->headerView;
}

void CommCtrlHeaderViewBackend::SetBounds(const Math::RectD &bounds)
{
	CommCtrlWidgetBackend::SetBounds(bounds);
	this->AutoSizeColumns();
}

void CommCtrlHeaderViewBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR;
}

//Private methods
void CommCtrlHeaderViewBackend::AutoSizeColumns()
{
	SizeD size = this->headerView.GetSize();

	uint32 count = this->GetItemCount();
	for (uint32 i = 0; i < count; i++)
	{
		HDITEMW hdi;

		hdi.mask = HDI_WIDTH;
		hdi.cxy = size.width / count;

		this->SendMessage(HDM_SETITEMW, i, (LPARAM)&hdi);
	}
}





//NOT IMPLEMENTED
void _stdxx_::CommCtrlHeaderViewBackend::UpdateSelection() const
{
	NOT_IMPLEMENTED_ERROR;
}
//NOT IMPLEMENTED
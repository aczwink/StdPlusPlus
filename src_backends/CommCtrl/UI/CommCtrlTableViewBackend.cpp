/*
* Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include "CommCtrlTableViewBackend.hpp"
//Local
#include <Std++/UI/Controllers/TreeController.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Public methods
void CommCtrlTableViewBackend::ControllerChanged()
{
	//clear all
	this->SendMessage(LVM_DELETEALLITEMS, 0, 0);

	//delete all columns
	uint32 nOldCols = this->GetNumberOfColsInView();
	while (nOldCols--)
	{
		this->SendMessage(LVM_DELETECOLUMN, nOldCols, 0);
	}


	if (this->tableView.HasController())
	{
		const SharedPointer<TreeController>& controller = this->tableView.GetController();

		const uint32 nRows = controller->GetNumberOfChildren();
		const uint32 nCols = controller->GetNumberOfColumns();

		//add col headers
		for (uint32 i = 0; i < nCols; i++)
		{
			LVCOLUMNW lvc;

			String text = controller->GetColumnText(i);

			lvc.mask = LVCF_FMT | LVCF_TEXT;
			lvc.pszText = (LPWSTR)text.ToUTF16().GetRawZeroTerminatedData();
			lvc.fmt = LVCFMT_LEFT;
			this->SendMessage(LVM_INSERTCOLUMNW, i, (LPARAM)&lvc);
		}

		for (uint32 i = 0; i < nRows; i++)
		{
			const ControllerIndex rowIndex = controller->GetChildIndex(i, 0);
			
			LVITEMW item;
			item.mask = 0;
			item.iItem = i;
			item.iSubItem = 0;
			
			this->SendMessage(LVM_INSERTITEMW, 0, (LPARAM)&item);

			for (uint32 j = 0; j < nCols; j++)
			{
				const ControllerIndex childIndex = controller->GetChildIndex(i, j);
				String text = controller->GetText(childIndex);

				item.iSubItem = j;
				item.pszText = (LPWSTR)text.ToUTF16().GetRawZeroTerminatedData(); //LPSTR_TEXTCALLBACKW;
				item.mask = LVIF_TEXT;

				this->SendMessage(LVM_SETITEMW, 0, (LPARAM)&item);
			}
		}
	}

	this->AutoSizeColumns();
}

Math::SizeD CommCtrlTableViewBackend::GetSizeHint() const
{
	return StdXX::Math::SizeD();
}

Widget & CommCtrlTableViewBackend::GetWidget()
{
	return this->tableView;
}

const Widget & CommCtrlTableViewBackend::GetWidget() const
{
	return this->tableView;
}

void CommCtrlTableViewBackend::OnMessage(WinMessageEvent& event)
{
	switch (event.message)
	{
	default:
		CommCtrlWidgetBackend::OnMessage(event);
	}
}

void CommCtrlTableViewBackend::SetBounds(const Math::RectD &bounds)
{
	CommCtrlWidgetBackend::SetBounds(bounds);
	this->AutoSizeColumns();
}







//NOT IMPLEMENTED:
void _stdxx_::CommCtrlTableViewBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlTableViewBackend::UpdateSelection() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
//:NOT IMPLEMENTED
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
#include "CommCtrlSelectBoxBackend.hpp"
//Local
#include <Std++/UI/Controllers/TreeController.hpp>
#include <Std++/UI/Events/SelectionChangedEvent.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
void CommCtrlSelectBoxBackend::ControllerChanged()
{
	this->SendMessage(CB_RESETCONTENT, 0, 0);

	const SharedPointer<TreeController>& controller = this->selectBox.GetController();
	if (!controller.IsNull())
	{
		uint32 n = controller->GetNumberOfChildren();
		for (uint32 i = 0; i < n; i++)
		{
			ControllerIndex childIndex = controller->GetChildIndex(i, 0);
			this->AddItem(controller->GetText(childIndex));
		}
	}
}

SizeD CommCtrlSelectBoxBackend::GetSizeHint() const
{
	HDC hDC = GetDC(this->GetHWND());
	HFONT hFont = (HFONT)this->SendMessage(WM_GETFONT, 0, 0);
	SelectObject(hDC, hFont);

	uint16 x = 50; //minimum
	uint32 n = 0;
	if (this->selectBox.HasController())
	{
		const SharedPointer<TreeController>& controller = this->selectBox.GetController();
		n = controller->GetNumberOfChildren(); //number of items
		for (uint32 i = 0; i < n; i++)
		{
			wchar_t buffer[2048];
			this->SendMessage(CB_GETLBTEXT, i, (LPARAM)buffer);
			uint32 nChars = this->SendMessage(CB_GETLBTEXTLEN, i, 0);
			SIZE size;
			GetTextExtentPoint32W(hDC, buffer, nChars, &size);
			if (size.cx > x)
				x = (uint16)size.cx;
		}
	}

	ReleaseDC(this->GetHWND(), hDC);

	x += 10; //add some spacing left and right

	return SizeD(x, 21);
}

Widget & CommCtrlSelectBoxBackend::GetWidget()
{
	return this->selectBox;
}

const Widget & CommCtrlSelectBoxBackend::GetWidget() const
{
	return this->selectBox;
}

void CommCtrlSelectBoxBackend::OnMessage(WinMessageEvent& event)
{
	switch (event.message)
	{
	case WM_COMMAND:
	{
		switch (HIWORD(event.wParam))
		{
		case CBN_SELCHANGE:
		{
			const SharedPointer<TreeController>& controller = this->selectBox.GetController();
			if (!controller.IsNull())
			{
				uint32 row = this->SendMessage(CB_GETCURSEL, 0, 0);
				ControllerIndex index = controller->GetChildIndex(row, 0);

				LinkedList<ControllerIndex> selection;
				selection.InsertTail(index);

				SelectionChangedEvent e(Move(selection));
				this->selectBox.Event(e);
			}
			event.consumed = true;
			event.result = 0;
		}
		break;
		}
	}
	break;
	}
}

void CommCtrlSelectBoxBackend::SetHint(const StdXX::String & text)
{
	CommCtrlWidgetBackend::SetHint(text);

	this->SendMessage(CB_SETCUEBANNER, 0, (LPARAM)text.ToUTF16().GetRawZeroTerminatedData());
}

void CommCtrlSelectBoxBackend::UpdateSelection() const
{
	const auto& selection = this->selectBox.GetSelectionController().GetSelectedIndexes();
	WPARAM index = -1;
	if (!selection.IsEmpty())
		index = selection.GetFront().GetRow();
	this->SendMessage(CB_SETCURSEL, index, 0);
}













//TODO: NOT IMPLEMENTED:
void _stdxx_::CommCtrlSelectBoxBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implment me
}
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
#include "CommCtrlCheckBoxBackend.hpp"
//Local
#include <Std++/UI/Controls/CheckBox.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Public methods
SizeD CommCtrlCheckBoxBackend::GetSizeHint() const
{
	SizeD checkSize;
	checkSize.width = GetSystemMetrics(SM_CXMENUCHECK);
	checkSize.height = GetSystemMetrics(SM_CYMENUCHECK);
	return checkSize + this->GetTextExtents().Cast<float64>();
}

Widget &CommCtrlCheckBoxBackend::GetWidget()
{
	return *this->checkBox;
}

const Widget & CommCtrlCheckBoxBackend::GetWidget() const
{
	return *this->checkBox;
}

bool CommCtrlCheckBoxBackend::IsChecked() const
{
	return this->SendMessage(BM_GETCHECK, 0, 0) == BST_CHECKED;
}

void CommCtrlCheckBoxBackend::OnMessage(WinMessageEvent& event)
{
	switch (event.message)
	{
	case WM_COMMAND:
	{
		switch (HIWORD(event.wParam))
		{
		case BN_CLICKED:
		{
			if (this->checkBox->onToggledHandler.IsBound())
				this->checkBox->onToggledHandler();
			event.consumed = true;
			event.result = 0;
		}
		break;
		}
	}
	break;
	}
}

void CommCtrlCheckBoxBackend::SetText(const String & text)
{
	Win32Window::SetText(text);
}




//not implemented
void _stdxx_::CommCtrlCheckBoxBackend::Repaint()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlCheckBoxBackend::Select(StdXX::UI::ControllerIndex & controllerIndex) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlCheckBoxBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlCheckBoxBackend::SetHint(const StdXX::String & text) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlCheckBoxBackend::UpdateSelection(StdXX::UI::SelectionController & selectionController) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlCheckBoxBackend::ResetView() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
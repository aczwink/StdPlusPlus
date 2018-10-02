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
//Local
#include <Std++/_Backends/UI/ViewBackend.hpp>
#include <Std++/UI/Views/SelectBox.hpp>
#include "Win32Window.hpp"

namespace _stdxx_
{
	/*
	https://docs.microsoft.com/en-us/windows/desktop/controls/combo-boxes
	*/
	class CommCtrlSelectBoxBackend : public ViewBackend, public CommCtrlWidgetBackend, public Win32Window
	{
	public:
		//Constructor
		inline CommCtrlSelectBoxBackend(StdXX::UIBackend& uiBackend, StdXX::UI::SelectBox& selectBox) : ViewBackend(uiBackend), CommCtrlWidgetBackend(uiBackend), WidgetBackend(uiBackend), Win32Window(*this, WC_COMBOBOXW, CBS_DROPDOWNLIST),
			selectBox(selectBox)
		{
		}

		//Methods
		void ControllerChanged() override;
		StdXX::Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget & GetWidget() override;
		const StdXX::UI::Widget & GetWidget() const override;
		void SetHint(const StdXX::String & text) override;
		void UpdateSelection() const override;

		//NOT IMPLEMENTED:
		void SetEditable(bool enable) const override;
		//END OF NOT IMPLEMENTED

	private:
		//Members
		StdXX::UI::SelectBox& selectBox;

		//Inline
		inline void AddItem(const StdXX::String& text)
		{
			this->SendMessage(CB_ADDSTRING, 0, (LPARAM)text.ToUTF16().GetRawZeroTerminatedData());
		}
	};
}
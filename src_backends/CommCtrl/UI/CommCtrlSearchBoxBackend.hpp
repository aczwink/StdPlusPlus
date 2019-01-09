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
#include <Std++/UI/Controls/SearchBox.hpp>
#include "CommCtrlWidgetBackend.hpp"
#include "Win32Window.hpp"

namespace _stdxx_
{
	/*
	WinAPI Documentation: https://msdn.microsoft.com/en-us/library/windows/desktop/bb775458(v=vs.85).aspx
	*/
	class CommCtrlSearchBoxBackend : public CommCtrlWidgetBackend, public Win32Window
	{
	public:
		//Constructor
		inline CommCtrlSearchBoxBackend(StdXX::UIBackend& uiBackend, StdXX::UI::SearchBox& searchBox) : CommCtrlWidgetBackend(uiBackend), WidgetBackend(uiBackend), Win32Window(*this, WC_EDITW, 0, WS_EX_CLIENTEDGE),
			searchBox(searchBox)
		{
			this->SendMessage(EM_SETCUEBANNER, FALSE, (LPARAM)L"Search...");
		}

		//Methods
		StdXX::Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget & GetWidget() override;
		const StdXX::UI::Widget & GetWidget() const override;

		//NOT IMPLEMENTED:
		void SetEditable(bool enable) const override;
		//:NOT IMPLEMENTED

	private:
		StdXX::UI::SearchBox& searchBox;
	};
}
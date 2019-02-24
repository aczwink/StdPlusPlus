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
//Local
#include <Std++/_Backends/UI/HeaderViewBackend.hpp>
#include "CommCtrlWidgetBackend.hpp"
#include "Win32Window.hpp"

namespace _stdxx_
{
	/*
	WinAPI doc: https://docs.microsoft.com/en-us/windows/desktop/controls/header-control-reference
	*/
	class CommCtrlHeaderViewBackend : public HeaderViewBackend, public CommCtrlWidgetBackend, public Win32Window
	{
	public:
		//Constructor
		inline CommCtrlHeaderViewBackend(StdXX::UIBackend& uiBackend, StdXX::UI::HeaderView& headerView) : WidgetBackend(uiBackend), HeaderViewBackend(uiBackend, headerView), CommCtrlWidgetBackend(uiBackend), Win32Window(*this, WC_HEADERW, HDS_BUTTONS | HDS_DRAGDROP | HDS_FULLDRAG | HDS_HORZ | HDS_HOTTRACK, WS_EX_CLIENTEDGE)
		{
		}

		//Methods
		StdXX::Math::SizeD GetSizeHint() const override;
		void SetEditable(bool enable) const override;
	};
}
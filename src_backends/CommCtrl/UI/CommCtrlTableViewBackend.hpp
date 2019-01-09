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
#include <Std++/UI/Views/TableView.hpp>
#include "Win32Window.hpp"

namespace _stdxx_
{
	/*
	WinAPI doc: https://docs.microsoft.com/de-de/windows/desktop/Controls/list-view-control-reference
	*/
	class CommCtrlTableViewBackend : public ViewBackend, public CommCtrlWidgetBackend, public Win32Window
	{
	public:
		//Constructor
		inline CommCtrlTableViewBackend(StdXX::UIBackend& uiBackend, StdXX::UI::TableView& tableView) : ViewBackend(uiBackend), CommCtrlWidgetBackend(uiBackend), WidgetBackend(uiBackend), Win32Window(*this, WC_LISTVIEWW, LVS_REPORT | LVS_SHOWSELALWAYS | WS_CLIPCHILDREN),
			tableView(tableView)
		{
			//LVS_EX_AUTOSIZECOLUMNS does not work-.-
			DWORD styles = LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE;
			this->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, styles, styles);
		}

		//Methods
		void ControllerChanged() override;
		StdXX::Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget & GetWidget() override;
		const StdXX::UI::Widget & GetWidget() const override;
		void OnMessage(WinMessageEvent& event) override;
		void SetBounds(const StdXX::Math::RectD &bounds) override;

		//NOT IMPLEMENTED:
		void SetEditable(bool enable) const override;
		void UpdateSelection() const override;
		//:NOT IMPLEMENTED

	private:
		//Members
		StdXX::UI::TableView& tableView;

		//Inline
		inline void AutoSizeColumns()
		{
			for (uint32 i = 0; i < this->GetNumberOfColsInView(); i++)
				this->SendMessage(LVM_SETCOLUMNWIDTH, i, LVSCW_AUTOSIZE_USEHEADER); //LVSCW_AUTOSIZE
		}

		inline uint32 GetNumberOfColsInView() const
		{
			HWND hHeader = (HWND)this->SendMessage(LVM_GETHEADER, 0, 0);
			return SendMessageW(hHeader, HDM_GETITEMCOUNT, 0, 0);
		}
	};
}
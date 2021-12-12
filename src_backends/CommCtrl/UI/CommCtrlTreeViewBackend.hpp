/*
* Copyright (c) 2018-2019,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include <Std++/SmartPointers/SharedPointer.hpp>
#include <Std++/UI/Views/TreeView.hpp>
#include "Win32Window.hpp"

namespace _stdxx_
{
	/*
	WinAPI doc: https://docs.microsoft.com/de-de/windows/desktop/Controls/tree-view-control-reference
	*/
	class CommCtrlTreeViewBackend : public ViewBackend, public CommCtrlWidgetBackend, public Win32Window
	{
	public:
		//Constructor
		inline CommCtrlTreeViewBackend(StdXX::UIBackend& uiBackend, StdXX::UI::TreeView& treeView) : ViewBackend(uiBackend), CommCtrlWidgetBackend(uiBackend), WidgetBackend(uiBackend), Win32Window(*this, WC_TREEVIEWW, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_TRACKSELECT | WS_BORDER),
			treeView(treeView)
		{
			this->SendMessage(TVM_SETEXTENDEDSTYLE, TVS_EX_DOUBLEBUFFER | TVS_EX_FADEINOUTEXPANDOS, TVS_EX_DOUBLEBUFFER | TVS_EX_FADEINOUTEXPANDOS);
		}

		//Methods
		void ControllerChanged() override;
		StdXX::Math::SizeD GetSizeHint() const override;
		StdXX::UI::Widget & GetWidget() override;
		const StdXX::UI::Widget & GetWidget() const override;
		void OnMessage(WinMessageEvent& event) override;

		//NOT IMPLEMENTED:
		void SetEditable(bool enable) const override;
		void UpdateSelection() const override;
		//:NOT IMPLEMENTED

	private:
		//Members
		StdXX::UI::TreeView& treeView;
		StdXX::BinaryTreeMap<HTREEITEM, StdXX::UI::ControllerIndex> treeItemMap;

		//Methods
		void AddItems(const StdXX::SharedPointer<StdXX::UI::TreeController>& controller, const StdXX::UI::ControllerIndex& nodeIndex, HTREEITEM hNode);
	};
}
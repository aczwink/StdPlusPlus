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
#include "CommCtrlTreeViewBackend.hpp"
//Local
#include <Std++/UI/Controllers/TreeController.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::UI;

//Public methods
void CommCtrlTreeViewBackend::ControllerChanged()
{
	//clear all
	this->SendMessage(TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
	this->treeItemMap.Release();

	if (this->treeView.HasController())
	{
		this->AddItems(this->treeView.GetController(), ControllerIndex(), TVI_ROOT);
	}
}

Math::SizeD CommCtrlTreeViewBackend::GetSizeHint() const
{
	return StdXX::Math::SizeD();
}

Widget & CommCtrlTreeViewBackend::GetWidget()
{
	return this->treeView;
}

const Widget & CommCtrlTreeViewBackend::GetWidget() const
{
	return this->treeView;
}

void CommCtrlTreeViewBackend::OnMessage(WinMessageEvent& event)
{
	switch (event.message)
	{
	case WM_NOTIFY:
	{
		NMHDR* header = (NMHDR*)event.lParam;
		switch (header->code)
		{
		case TVN_SELCHANGEDW:
		{
			NMTREEVIEWW* treeViewNotification = (NMTREEVIEWW*)header;

			LinkedList<ControllerIndex> selection;
			selection.InsertTail(this->treeItemMap[treeViewNotification->itemNew.hItem]);

			SelectionChangedEvent sce(Move(selection));
			this->treeView.Event(sce);
		}
		break;
		default:
			CommCtrlWidgetBackend::OnMessage(event);
		}
	}
	break;
	default:
		CommCtrlWidgetBackend::OnMessage(event);
	}
}

//Private methods
void CommCtrlTreeViewBackend::AddItems(const SharedPointer<TreeController>& controller, const ControllerIndex& nodeIndex, HTREEITEM hNode)
{
	for (uint32 i = 0; i < controller->GetNumberOfChildren(nodeIndex); i++)
	{
		ControllerIndex childIndex = controller->GetChildIndex(i, 0, nodeIndex);
		String text = controller->GetText(childIndex);
		
		TVITEMW tvi;
		tvi.mask = TVIF_TEXT;
		tvi.pszText = (LPWSTR)text.ToUTF16().GetRawZeroTerminatedData();
		tvi.cchTextMax = text.GetLength();

		TVINSERTSTRUCTW tvis;

		tvis.hParent = hNode;
		tvis.hInsertAfter = TVI_LAST;
		tvis.item = tvi;

		HTREEITEM hChild = (HTREEITEM)this->SendMessage(TVM_INSERTITEMW, 0, (LPARAM)&tvis);
		this->treeItemMap[hChild] = childIndex;

		this->AddItems(controller, childIndex, hChild);
	}
}










//NOT IMPLEMENTED:
void _stdxx_::CommCtrlTreeViewBackend::SetEditable(bool enable) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void _stdxx_::CommCtrlTreeViewBackend::UpdateSelection() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
//:NOT IMPLEMENTED
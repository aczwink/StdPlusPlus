/*
* Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
*
* This file is part of ACStdLib.
*
* ACStdLib is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ACStdLib is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
*/
//Class header
#include <ACStdLib/UI/Views/TreeView.hpp>
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include <ACStdLib/UI/WidgetContainer.hpp>
#include <ACStdLib/UI/Controllers/TreeController.hpp>
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//https://msdn.microsoft.com/en-us/library/windows/desktop/bb759988(v=vs.85).aspx

//Local functions
static HTREEITEM InsertItemAtFront(HWND hWnd, HTREEITEM hItem, const String &refText)
{
	TVINSERTSTRUCTW tvis;

	tvis.hParent = hItem;
	tvis.hInsertAfter = TVI_FIRST;

	//item
	tvis.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvis.item.pszText = (LPWSTR)refText.ToUTF16().GetRawZeroTerminatedData();
	tvis.item.cchTextMax = refText.GetLength();
	//tvis.item.lParam = (LPARAM)pNode;

	return (HTREEITEM)SendMessage(hWnd, TVM_INSERTITEMW, 0, (LPARAM)&tvis);
}

static void AddNodes(HWND hWnd, HTREEITEM hItem, const ControllerIndex &parent, TreeController &controller)
{
	uint32 nChildren, i, index;
	HTREEITEM hChild;

	nChildren = controller.GetNumberOfChildren(parent);
	for(i = 0; i < nChildren; i++)
	{
		index = nChildren - i - 1; //we always insert from back to front because the tree view is slow in appending to the back

		ControllerIndex childIndex = controller.GetChildIndex(index, 0, parent);
		hChild = InsertItemAtFront(hWnd, hItem, controller.GetText(childIndex));

		AddNodes(hWnd, hChild, childIndex, controller);
	}
}

//Eventhandlers
/*
void TreeView::OnModelChanged()
{
	TreeView_DeleteAllItems((HWND)this->backend);

	if (this->controller)
	{
		AddNodes((HWND)this->backend, nullptr, ControllerIndex(), *this->controller);
	}
}*/


/*
OLD:

void *TreeView::GetSelectedNode() const
{
	TVITEMW tvi;

	tvi.hItem = TreeView_GetSelection((HWND)this->systemHandle);
	if(!tvi.hItem)
		return nullptr;

	tvi.mask = TVIF_PARAM;

	SendMessage((HWND)this->systemHandle, TVM_GETITEMW, 0, (LPARAM)&tvi);

	return (void *)tvi.lParam;
}
*/
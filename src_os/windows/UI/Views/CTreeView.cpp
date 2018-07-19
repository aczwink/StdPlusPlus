/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Views/TreeView.hpp>
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include <Std++/UI/WidgetContainer.hpp>
#include <Std++/UI/Controllers/TreeController.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

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

 void TreeView::CreateOSWindow()
-{
-	this->systemHandle = CreateWindowExA(WS_EX_CLIENTEDGE, WC_TREEVIEWA, nullptr, WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, 0, 0, 0, 0, GET_HWND(this->GetParent()->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
-	SetWindowLongPtr((HWND)this->systemHandle, GWLP_USERDATA, (LONG_PTR)this);
-
-	SendMessage((HWND)this->systemHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
-	TreeView_SetExtendedStyle((HWND)this->systemHandle, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
-}

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
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
#include <ACStdLib/UI/Controllers/Controller.hpp>
#include "../CFullAccessWidget.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//https://msdn.microsoft.com/en-us/library/windows/desktop/bb759988(v=vs.85).aspx

//Local functions
static HTREEITEM InsertItemAtFront(HWND hWnd, HTREEITEM hItem, void *pNode, const String &refText)
{
	TVINSERTSTRUCTW tvis;
	UTF16String textUTF16;

	textUTF16 = refText.GetUTF16();

	tvis.hParent = hItem;
	tvis.hInsertAfter = TVI_FIRST;

	//item
	tvis.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvis.item.pszText = (LPWSTR)textUTF16.GetC_Str();
	tvis.item.cchTextMax = textUTF16.GetLength();
	tvis.item.lParam = (LPARAM)pNode;

	return (HTREEITEM)SendMessage(hWnd, TVM_INSERTITEMW, 0, (LPARAM)&tvis);
}

static void AddNodes(HWND hWnd, HTREEITEM hItem, void *pNode, ATreeController &refController)
{
	uint32 nChildren, i, index;
	void *pChildNode;
	HTREEITEM hChild;

	nChildren = refController.GetNumberOfChildren(pNode);
	for(i = 0; i < nChildren; i++)
	{
		index = nChildren - i - 1; //we always insert from back to front because the tree view is slow in appending to the back

		pChildNode = refController.GetChild(pNode, index);
		hChild = InsertItemAtFront(hWnd, hItem, pChildNode, refController.GetText(pChildNode));

		AddNodes(hWnd, hChild, pChildNode, refController);
	}
}

//Eventhandlers
void CTreeView::OnModelChanged()
{
	TreeView_DeleteAllItems((HWND)this->systemHandle);

	if(this->pController)
	{
		AddNodes((HWND)this->systemHandle, nullptr, nullptr, *this->pController);
	}
}

//Private methods
void CTreeView::CreateOSWindow()
{
	this->systemHandle = CreateWindowExA(WS_EX_CLIENTEDGE, WC_TREEVIEWA, nullptr, WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, 0, 0, 0, 0, GET_HWND(this->GetParent()->GetWindow()), nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr((HWND)this->systemHandle, GWLP_USERDATA, (LONG_PTR)this);

	SendMessage((HWND)this->systemHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
	TreeView_SetExtendedStyle((HWND)this->systemHandle, TVS_EX_DOUBLEBUFFER, TVS_EX_DOUBLEBUFFER);
}

//Public methods
void *CTreeView::GetSelectedNode() const
{
	TVITEMW tvi;

	tvi.hItem = TreeView_GetSelection((HWND)this->systemHandle);
	if(!tvi.hItem)
		return nullptr;

	tvi.mask = TVIF_PARAM;

	SendMessage((HWND)this->systemHandle, TVM_GETITEMW, 0, (LPARAM)&tvi);

	return (void *)tvi.lParam;
}
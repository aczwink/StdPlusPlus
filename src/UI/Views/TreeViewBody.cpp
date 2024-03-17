/*
 * Copyright (c) 2019-2024 Amir Czwink (amir130@hotmail.de)
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
#include "TreeViewBody.hpp"
//Local
#include <Std++/SmartPointers/UniquePointer.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;

//Constructor
TreeViewBody::TreeViewBody(const HeaderView& headerView, const StdXX::UI::SelectionController& selectionController) : headerView(headerView), selectionController(selectionController)
{
	this->styleContext.AddType(u8"TreeViewContent");

	this->root = nullptr;
	this->top = nullptr;
}

//Destructor
TreeViewBody::~TreeViewBody()
{
	delete this->root;
}

//Private methods
void TreeViewBody::DrawCell(Painter & painter, Cell & cell, const RectD & rect, bool isFirstCol, const StdXX::UI::StyleProperties& rowProps)
{
	//draw foreground
	painter.SetFillColor(rowProps.Color());

	String text = this->controller->GetText(cell.controllerIndex);
	Rect textRect = rect;
	textRect.y() += rowProps.PaddingBottom();
	textRect.height() = rect.height() - rowProps.PaddingTop();
	painter.DrawText(rect.origin, text);
}

void TreeViewBody::DrawRow(Painter & painter, Row & row, float64 top)
{
	const UI::StyleProperties& rowProps = (this->selectionController.IsSelected(row.controllerIndex)) ? this->selectedRowProps : this->rowProps;

	//draw background
	RectD rowRect;
	rowRect.y() = top - row.height;
	rowRect.height() = row.height;
	rowRect.width() = this->GetSize().width;

	painter.Rectangle(rowRect);
	painter.SetFillColor(this->StyleProperties().BackgroundColor());
	painter.Fill();

	//draw cells
	uint32 nCols = this->controller->GetNumberOfColumns();
	for (uint32 i = 0; i < nCols; i++)
	{
		RectD cellRect = this->headerView.GetItemRect(i);
		cellRect.y() = rowRect.y();
		cellRect.height() = rowRect.height();

		this->DrawCell(painter, row.items[i], cellRect, i == 0, rowProps);
	}
}

Row* TreeViewBody::NextVisible(Row* row) const
{
	if (row->isExpanded)
	{
		if ((row->firstChild == nullptr) && (row->nChildren > 0))
		{
			//buffer first child
			Row* r = new Row;
			row->firstChild = r;
			r->parent = row;
			r->controllerIndex = this->controller->GetChildIndex(0, 0, row->controllerIndex);
			r->nChildren = this->controller->GetNumberOfChildren(r->controllerIndex);
		}

		return row->firstChild;
	}

	if ((row->next == nullptr) && (row->controllerIndex.GetRow() < (row->parent->nChildren - 1)))
	{
		//buffer next sibling
		Row* r = new Row;
		row->next = r;
		r->parent = row->parent;
		r->controllerIndex = this->controller->GetChildIndex(row->controllerIndex.GetRow()+1, 0, row->parent->controllerIndex);
	}
	
	return row->next;
}

void TreeViewBody::RequireCellsBuffered(Row& row) const
{
	if (!row.items.IsEmpty())
		return;

	uint32 nCols = this->controller->GetNumberOfColumns();
	row.items.Resize(nCols);
	for (uint32 i = 0; i < nCols; i++)
	{
		row.items[i].controllerIndex = this->controller->GetChildIndex(row.controllerIndex.GetRow(), i, row.controllerIndex.GetParent());
	}
}

void TreeViewBody::RequireHeight(Row& row, Painter& painter) const
{
	if (row.height != 0)
		return; //height is already computed

	this->RequireCellsBuffered(row);

	uint32 nCols = this->controller->GetNumberOfColumns();
	for (uint32 i = 0; i < nCols; i++)
	{
		String text = this->controller->GetText(row.items[i].controllerIndex);
		SizeD textSize = painter.ComputeTextSize(text);
		row.height = Math::Max(row.height, textSize.height);
	}

	row.height += this->rowProps.PaddingTop() + this->rowProps.PaddingBottom();
}

//Event handlers
void TreeViewBody::OnPaint(PaintEvent& event)
{
	//query styles
	UI::StyleContext rowContext;
	rowContext.AddType(u8"row");
	this->rowProps = StyleSheet::Global().QueryVirtual(rowContext, *this);
	
	rowContext.AddPseudoClass(u8"selected");
	this->selectedRowProps = StyleSheet::Global().QueryVirtual(rowContext, *this);

	//if we have no root, get it
	if (this->root == nullptr)
	{
		if (!this->controller.IsNull())
		{
			this->root = new Row();
			this->root->nChildren = this->controller->GetNumberOfChildren();
			this->root->isExpanded = true;
			this->top = this->NextVisible(this->root);
		}
	}

	if (this->selectionController.GetSelectedIndexes().IsEmpty())
	{
		LinkedList<ControllerIndex> sel;
		Row* row = this->NextVisible(this->root);
		row = this->NextVisible(row);
		sel.InsertTail(row->controllerIndex);
		SelectionChangedEvent e(StdXX::Move(sel));

		this->GetParent()->Event(e);
	}

	UniquePointer<Painter> painter = this->CreatePainter(event.osHandle);

	//skip rows until we come to the ones we should paint
	Row* row = this->top;
	float64 top = this->GetSize().height;
	while (top > event.GetUpdateRect().GetVerticalEnd())
	{
		this->RequireHeight(*row, *painter);
		top -= row->height;
		row = this->NextVisible(row);
	}

	//draw rows
	while ((top > event.GetUpdateRect().y()) && row)
	{
		this->RequireHeight(*row, *painter);
		this->DrawRow(*painter, *row, top);
		top -= row->height;
		row = this->NextVisible(row);
	}

	//draw leftover background
	if (top > event.GetUpdateRect().y())
	{
		RectD leftover = event.GetUpdateRect();
		leftover.height() = top - leftover.y();

		const UI::StyleProperties& props = this->StyleProperties();

		//draw background
		painter->Rectangle(leftover);
		painter->SetFillColor(props.BackgroundColor());
		painter->Fill();
	}
}

void TreeViewBody::OnResized()
{
	this->Repaint();
}
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
TreeViewBody::TreeViewBody(const HeaderView& headerView) : headerView(headerView)
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
void TreeViewBody::DrawCell(Painter & painter, Cell & cell, const RectD & rect, bool isFirstCol)
{
	const UI::StyleProperties& props = this->StyleProperties();

	//draw background
	painter.Rectangle(rect);
	painter.SetFillColor(props.BackgroundColor());
	painter.Fill();

	//draw foreground
	painter.SetFillColor(props.Color());

	String text = this->controller->GetText(cell.controllerIndex);
	painter.DrawText(rect.origin, text);
}

void TreeViewBody::DrawRow(Painter & painter, Row & row, float64 top)
{
	uint32 nCols = this->controller->GetNumberOfColumns();
	for (uint32 i = 0; i < nCols; i++)
	{
		RectD cellRect = this->headerView.GetItemRect(i);
		cellRect.y() = top - row.height;
		cellRect.height() = row.height;

		this->DrawCell(painter, row.items[i], cellRect, i == 0);
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

	if ((row->next == nullptr) && (row->controllerIndex.GetRow() < row->parent->nChildren))
	{
		//buffer next sibling
		Row* r = new Row;
		row->next = r;
		r->parent = row->parent;
		r->controllerIndex = this->controller->GetChildIndex(row->controllerIndex.GetRow(), 0, row->parent->controllerIndex);
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
}

//Event handlers
void TreeViewBody::OnPaint(PaintEvent& event)
{
	//if we have no root, get it
	if (this->root == nullptr)
	{
		this->root = new Row();
		if (!this->controller.IsNull())
			this->root->nChildren = this->controller->GetNumberOfChildren();
		this->root->isExpanded = true;
		this->top = this->root;
	}

	UniquePointer<Painter> painter = this->drawableBackend->CreatePainter();

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
}

void TreeViewBody::OnResized()
{
	this->Repaint();
}
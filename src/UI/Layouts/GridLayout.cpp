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
#include <Std++/UI/Layouts/GridLayout.hpp>
//Local
#include <Std++/Containers/Map/Map.hpp>
#include <Std++/Streams/StdOut.hpp>
#include <Std++/UI/Containers/CompositeWidget.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
GridLayout::GridLayout()
{
    this->nColumns = 1;
    this->nRows = 1;
    this->fillVertical = false;
    this->horzGap = 5;
    this->vertGap = 5;
    this->margin = 5;
}

//Private methods
void GridLayout::CanExpand(const Widget *widget, bool &expandHorz, bool &expandVert)
{
	if(widget->sizingPolicy.GetHorizontalAttributes().expand)
		expandHorz = true;

	if(widget->sizingPolicy.GetVerticalAttributes().expand)
		expandVert = true;

	if(expandHorz && expandVert)
		return; //no need to search further

	if(const CompositeWidget *container = dynamic_cast<const CompositeWidget *>(widget))
	{
		for(const Widget *const& child : container->GetChildren())
		{
			this->CanExpand(child, expandHorz, expandVert);

			if(expandHorz && expandVert)
				return; //no need to search further
		}
	}
}

Math::SizeD GridLayout::ComputeSizingInfo(const CompositeWidget &refContainer, DynamicArray<uint16> &refColumnWidths, DynamicArray<uint16> &refRowHeights)
{
    uint8 col, row;
    uint16 i;
    Math::SizeD tmp, totalSize;

    if(refContainer.GetChildren().IsEmpty())
        return Math::SizeD();

    //check if we have enough space
    this->EnsureGridBigEnough((uint8)refContainer.GetChildren().GetNumberOfElements());
	refColumnWidths.Resize(this->nColumns);
	refRowHeights.Resize(this->nRows);

    //set all widths and heights to 0
	for (uint16 x = 0; x < this->nColumns; x++)
		refColumnWidths[x] = 0;
	for (uint16 y = 0; y < this->nRows; y++)
		refRowHeights[y] = 0;
	
	//update column widths and heights according to children
    row = 0;
    col = 0;
    for(const Widget *const& refpWidget : refContainer.GetChildren())
    {
        tmp = refpWidget->GetSizeHint();
        if(refColumnWidths[col] < tmp.width)
            refColumnWidths[col] = tmp.width;
        if(refRowHeights[row] < tmp.height)
            refRowHeights[row] = tmp.height;

        col++;
        if(col == this->nColumns)
        {
            col = 0;
            row++;
        }
    }

    //calculate total sizes
    for(i = 0; i < this->nColumns; i++)
        totalSize.width += refColumnWidths[i];
    for(i = 0; i < this->nRows; i++)
        totalSize.height += refRowHeights[i];

    return totalSize + Math::SizeD((this->nColumns - 1) * this->horzGap, (this->nRows - 1) * this->vertGap);
}

void GridLayout::DistributeLeftOverSize(const CompositeWidget &refContainer, const Math::SizeD &refMinSize, DynamicArray<uint16> &refColumnWidths, DynamicArray<uint16> &refRowHeights)
{
    uint16 col, row, partSize, nExpandColParts, nExpandRowParts, nGrowColParts, nGrowRowParts, max, maxIdx;
    Math::SizeD leftOver;
    Math::RectD rc;
    Map<uint16, uint8> expandCols, expandRows, growCols, growRows;

    rc = this->GetPlacementRect(refContainer);

    //calculate left over size
    if(rc.width() > refMinSize.width)
        leftOver.width = rc.width() - refMinSize.width;
    else
        leftOver.width = 0;

    if(rc.height() > refMinSize.height)
        leftOver.height = rc.height() - refMinSize.height;
    else
        leftOver.height = 0;

    //check which rows and cols can take extra space
    col = 0;
    row = 0;
    for(Widget *const& widget : refContainer.GetChildren())
    {
		bool expandHorz = false, expandVert = false;

		this->CanExpand(widget, expandHorz, expandVert);
		if(expandHorz)
			expandCols.Insert(col, widget->sizingPolicy.horzScale);
		if(expandVert)
			expandRows.Insert(row, widget->sizingPolicy.vertScale);

		/*
        if(widget->sizingPolicy.GetHorizontalAttributes().grow)
            growCols.Insert(col, widget->sizingPolicy.horzScale);

        if(widget->sizingPolicy.GetVerticalAttributes().grow)
            growRows.Insert(row, widget->sizingPolicy.vertScale);*/

        col++;
        if(col == this->nColumns)
        {
            col = 0;
            row++;
        }
    }

    //compute maximum scalefactors
    nExpandColParts = 0;
    nExpandRowParts = 0;
    nGrowColParts = 0;
    nGrowRowParts = 0;

    for(const auto &refKV : expandCols)
        nExpandColParts += refKV.value;
    for(const auto &refKV : expandRows)
        nExpandRowParts += refKV.value;
    for(const auto &refKV : growCols)
        nGrowColParts += refKV.value;
    for(const auto &refKV : growRows)
        nGrowRowParts += refKV.value;

    //distribute left-over width
    if(!expandCols.IsEmpty())
    {
        max = 0;
        maxIdx = 0;
        partSize = leftOver.width / nExpandColParts;

        for(const auto &refKV : expandCols)
        {
            refColumnWidths[refKV.key] += partSize * refKV.value;

            if(refKV.value > max)
            {
                max = refKV.value;
                maxIdx = refKV.key;
            }
        }

        refColumnWidths[maxIdx] += fmod(leftOver.width, nExpandColParts);
    }
    else if(!growCols.IsEmpty())
    {
        max = 0;
        maxIdx = 0;
        partSize = leftOver.width / nGrowColParts;

        for(const auto &refKV : growCols)
        {
            refColumnWidths[refKV.key] += partSize * refKV.value;

            if(refKV.value > max)
            {
                max = refKV.value;
                maxIdx = refKV.key;
            }
        }

        refColumnWidths[maxIdx] += fmod(leftOver.width, nGrowColParts);
    }

    //distribute left-over height
    max = 0;
    maxIdx = 0;
    if(!expandRows.IsEmpty())
    {
        partSize = leftOver.height / nExpandRowParts;

        for(const auto &refKV : expandRows)
        {
            refRowHeights[refKV.key] += partSize * refKV.value;

            if(refKV.value > max)
            {
                max = refKV.value;
                maxIdx = refKV.key;
            }
        }

        refRowHeights[maxIdx] += fmod(leftOver.height, nExpandRowParts);
    }
    else if(!growRows.IsEmpty())
    {
        partSize = leftOver.height / nGrowRowParts;

        for(const auto &refKV : growRows)
        {
            refRowHeights[refKV.key] += partSize * refKV.value;

            if(refKV.value > max)
            {
                max = refKV.value;
                maxIdx = refKV.key;
            }
        }

        refRowHeights[maxIdx] += fmod(leftOver.height, nGrowRowParts);
    }
}

void GridLayout::EnsureGridBigEnough(uint8 nCells)
{
    if(this->nColumns * this->nRows < nCells)
    {
        if(this->fillVertical)
        {
            this->nRows = nCells / this->nColumns;
            if(this->nColumns * this->nRows < nCells)
                this->nRows++;
        }
        else
        {
            this->nColumns = nCells / this->nRows;
            if(this->nColumns * this->nRows < nCells)
                this->nColumns++;
        }
    }

    ASSERT(this->nColumns * this->nRows >= nCells, "If you see this, report to StdXX");
}

Math::RectD GridLayout::GetPlacementRect(const CompositeWidget &container) const
{
    Math::RectD rc;
	rc.size = container.GetSize();
	rc.Enlarge(-(int32)this->margin, -(int32)this->margin);

    return rc;
}

void GridLayout::PositionChild(Widget &widget, const Math::RectD &cellBounds)
{
	/*
	 * We can flexibly position and size the widget within the cell.
	 *
	 * As the whole cell is allocated to the widget anyway, there is no advantage in minimizing the widgets size within the cell.
	*/
    Math::RectD widgetBounds = cellBounds; //we give the widget the whole cell

    //in case we are not allowed to grow, we probably need to allocate less
    Math::SizeD sizeHint = widget.GetSizeHint();
    if(!widget.sizingPolicy.GetHorizontalAttributes().grow && sizeHint.width < widgetBounds.width())
    {
        widgetBounds.width() = sizeHint.width;
        widgetBounds.x() = cellBounds.x();
    }
    if(!widget.sizingPolicy.GetVerticalAttributes().grow && sizeHint.height < widgetBounds.height())
    {
    	//widget is not allowed to grow vertically
		widgetBounds.height() = sizeHint.height;
		widgetBounds.y() = cellBounds.GetVerticalEnd() - widgetBounds.height();

        //fixed height widgets like sliders
		//put them in vertical center
		//widgetBounds.y() += (cellBounds.height() - sizeHint.height) / 2;
    }

	widget.SetBounds(widgetBounds);
}

//Public methods
Math::SizeD GridLayout::GetPreferredSize(const CompositeWidget &refContainer)
{
    Math::SizeD tmp;
    DynamicArray<uint16> columnWidths, rowHeights;

    tmp = this->ComputeSizingInfo(refContainer, columnWidths, rowHeights);
    tmp += Math::SizeD(this->margin, this->margin) * 2;

    return tmp;
}

void GridLayout::Layout(CompositeWidget &refContainer)
{
    uint16 x, y, col, row;
    Math::SizeD minSize, tmp;
    Math::RectD rc;
    DynamicArray<uint16> columnWidths, rowHeights;

    //collect info
    rc = this->GetPlacementRect(refContainer);
    minSize = this->ComputeSizingInfo(refContainer, columnWidths, rowHeights);

    //ASSERT(minSize.width <= rc.width(), u8"TODO: Can't do that allocation. Need to shrink widgets");
	//ASSERT(minSize.height <= rc.height(), u8"TODO: Can't do that allocation. Need to shrink widgets");

    this->DistributeLeftOverSize(refContainer, minSize, columnWidths, rowHeights);

    //layout children
    if(!refContainer.GetChildren().IsEmpty())
    {
        col = 0;
        row = 0;
        x = rc.x();
        y = rc.GetVerticalEnd() - rowHeights[0];
        for (Widget *const &refpWidget : refContainer.GetChildren())
        {
            if (col == this->nColumns)
            {
                col = 0;
                row++;

                x = rc.x();
                y -= rowHeights[row] + this->vertGap;
            }
            this->PositionChild(*refpWidget, Math::RectD(x, y, columnWidths[col], rowHeights[row]));

            x += columnWidths[col] + this->horzGap;
            col++;
        }
    }
}
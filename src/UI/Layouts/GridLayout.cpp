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
#include <ACStdLib/UI/Layouts/GridLayout.hpp>
//Local
#include <ACStdLib/Containers/Map/Map.hpp>
#include <ACStdLib/UI/WidgetContainer.h>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

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
/*
Computes the minimum sizes of the rows and columns in the grid.
*/
Size GridLayout::ComputeSizingInfo(const WidgetContainer &refContainer, DynamicArray<uint16> &refColumnWidths, DynamicArray<uint16> &refRowHeights)
{
    uint8 col, row;
    uint16 i;
    Size tmp, totalSize;

    if(refContainer.GetChildren().IsEmpty())
        return Size();

    //check if we have enough space
    this->EnsureGridBigEnough((uint8)refContainer.GetChildren().GetNumberOfElements());

    //set all widths and heights to 0
    refColumnWidths.Resize(refContainer.GetChildren().GetNumberOfElements());
    refRowHeights.Resize(refColumnWidths.GetNumberOfElements());

    for(i = 0; i < refColumnWidths.GetNumberOfElements(); i++)
    {
        refColumnWidths[i] = 0;
        refRowHeights[i] = 0;
    }

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

    return totalSize + Size((this->nColumns - 1) * this->horzGap, (this->nRows - 1) * this->vertGap);
}

void GridLayout::DistributeLeftOverSize(const WidgetContainer &refContainer, const Size &refMinSize, DynamicArray<uint16> &refColumnWidths, DynamicArray<uint16> &refRowHeights)
{
    uint16 col, row, partSize, nExpandColParts, nExpandRowParts, nGrowColParts, nGrowRowParts, max, maxIdx;
    Size leftOver;
    Rect rc;
    Map<uint16, uint8> expandCols, expandRows, growCols, growRows;

    rc = this->GetChildrenRect(refContainer);

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
    for(Widget *const& refpWidget : refContainer.GetChildren())
    {
        if(refpWidget->sizingPolicy.GetHorizontalAttributes().expand)
            expandCols.Insert(col, refpWidget->sizingPolicy.horzScale);

        if(refpWidget->sizingPolicy.GetVerticalAttributes().expand)
            expandRows.Insert(row, refpWidget->sizingPolicy.vertScale);

        if(refpWidget->sizingPolicy.GetHorizontalAttributes().grow)
            growCols.Insert(col, refpWidget->sizingPolicy.horzScale);

        if(refpWidget->sizingPolicy.GetVerticalAttributes().grow)
            growRows.Insert(row, refpWidget->sizingPolicy.vertScale);

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

        refColumnWidths[maxIdx] += leftOver.width % nExpandColParts;
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

        refColumnWidths[maxIdx] += leftOver.width % nGrowColParts;
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

        refRowHeights[maxIdx] += leftOver.height % nExpandRowParts;
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

        refRowHeights[maxIdx] += leftOver.height % nGrowRowParts;
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

    ASSERT(this->nColumns * this->nRows >= nCells);
}

Rect GridLayout::GetChildrenRect(const WidgetContainer &refContainer) const
{
    Rect rc;

    rc = refContainer.GetChildrenRect();
    rc.Enlarge(-(int32)this->margin, -(int32)this->margin);

    return rc;
}

void GridLayout::PositionChild(Widget &refWidget, const Rect &refBounds)
{
    Size sizeHint;
    Rect widgetBounds;

    widgetBounds = refBounds;
    sizeHint = refWidget.GetSizeHint();

    /*
    if(!refWidget.sizingPolicy.GetHorizontalAttributes().grow && sizeHint.width < widgetBounds.width)
    {
        widgetBounds.width = sizeHint.width;
        widgetBounds.x = (refBounds.width - sizeHint.width) / 2;
    }
    */

    if(!refWidget.sizingPolicy.GetVerticalAttributes().grow && sizeHint.height < widgetBounds.height())
    {
        widgetBounds.height() = sizeHint.height;
        //widgetBounds.y = (refBounds.height - sizeHint.height) / 2;
    }

    refWidget.SetRect(widgetBounds);
}

//Public methods
Size GridLayout::GetPreferredSize(const WidgetContainer &refContainer)
{
    Size tmp;
    DynamicArray<uint16> columnWidths, rowHeights;

    tmp = this->ComputeSizingInfo(refContainer, columnWidths, rowHeights);
    tmp += Size(this->margin, this->margin) * 2;

    return tmp;
}

void GridLayout::Layout(WidgetContainer &refContainer)
{
    uint16 x, y, col, row;
    Size minSize, tmp;
    Rect rc;
    DynamicArray<uint16> columnWidths, rowHeights;

    //collect info
    rc = this->GetChildrenRect(refContainer);
    minSize = this->ComputeSizingInfo(refContainer, columnWidths, rowHeights);

    this->DistributeLeftOverSize(refContainer, minSize, columnWidths, rowHeights);

    //layout children
    col = 0;
    row = 0;
    x = rc.x();
    y = rc.y();
    for(Widget *const& refpWidget : refContainer.GetChildren())
    {
        this->PositionChild(*refpWidget, Rect(x, y, columnWidths[col], rowHeights[row]));

        x += columnWidths[col] + this->horzGap;
        col++;
        if(col == this->nColumns)
        {
            x = rc.x();
            y += rowHeights[row] + this->vertGap;

            col = 0;
            row++;
        }
    }

    /*

    row = 0;
    col = 0;
    preferredSize = this->GetPreferredSize(refContainer);
    scaleHorz = rc.width / (float64)preferredSize.width;
    scaleVert = rc.height / (float64)preferredSize.height;

    //update column widths and heights according to children
    for(const Widget *const& refpWidget : refContainer.GetChildren())
    {
        tmp = refpWidget->GetSizeHint();
        tmp.width = (uint16)(scaleHorz * tmp.width);
        tmp.height = (uint16)(scaleVert * tmp.height);

        if(columnWidths[col] < tmp.width)
            columnWidths[col] = tmp.width;
        if(rowHeights[row] < tmp.height)
            rowHeights[row] = tmp.height;

        col++;
        if(col == this->nColumns)
        {
            col = 0;
            row++;
        }
    }
    */
}
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
#pragma once
//Local
#include "../../Containers/CArray/CArray.h"
#include "ILayout.h"

namespace ACStdLib
{
    namespace UI
    {
        class ACSTDLIB_API CGridLayout : public ILayout
        {
        private:
            //Members
            uint8 horzGap;
            uint8 vertGap;

            //Methods
            CSize ComputeSizingInfo(const AWidgetContainer &refContainer, CArray<uint16> &refColumnWidths, CArray<uint16> &refRowHeights);
            void DistributeLeftOverSize(const AWidgetContainer &refContainer, const CSize &refMinSize, CArray<uint16> &refColumnWidths, CArray<uint16> &refRowHeights);
            void EnsureGridBigEnough(uint8 nCells);
            CRect GetChildrenRect(const AWidgetContainer &refContainer) const;
            void PositionChild(AWidget &refWidget, const CRect &refBounds);

        protected:
            //Members
            uint8 nColumns;
            uint8 nRows;

        public:
            //Members
            bool fillVertical; //add rows or columns when grid too small
            uint8 margin;

            //Constructor
            CGridLayout();

            //Methods
            CSize GetPreferredSize(const AWidgetContainer &refContainer);
            void Layout(AWidgetContainer &refContainer);
        };

        class ACSTDLIB_API CHorizontalLayout : public CGridLayout
        {
        public:
            //Constructor
            inline CHorizontalLayout()
            {
                this->nRows = 1;
                this->fillVertical = false;
            }
        };

        class ACSTDLIB_API CVerticalLayout : public CGridLayout
        {
        public:
            //Constructor
            inline CVerticalLayout()
            {
                this->nColumns = 1;
                this->fillVertical = true;
            }
        };
    }
}
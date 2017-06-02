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
#include "../../Containers/DynamicArray/DynamicArray.hpp"
#include "ILayout.h"

namespace ACStdLib
{
    namespace UI
    {
        class ACSTDLIB_API GridLayout : public ILayout
        {
        private:
            //Members
            uint8 horzGap;
            uint8 vertGap;

            //Methods
            void CanExpand(const Widget *widget, bool &expandHorz, bool &expandVert);
			/**
			 * Computes the minimum sizes of the rows and columns in the grid.
			 *
			 * @param refContainer
			 * @param refColumnWidths
			 * @param refRowHeights
			 * @return
			 */
			Size ComputeSizingInfo(const WidgetContainer &refContainer, DynamicArray<uint16> &refColumnWidths, DynamicArray<uint16> &refRowHeights);
            void DistributeLeftOverSize(const WidgetContainer &refContainer, const Size &refMinSize, DynamicArray<uint16> &refColumnWidths, DynamicArray<uint16> &refRowHeights);
            void EnsureGridBigEnough(uint8 nCells);
            Rect GetChildrenRect(const WidgetContainer &refContainer) const;
            void PositionChild(Widget &refWidget, const Rect &bounds);

        protected:
            //Members
            uint8 nColumns;
            uint8 nRows;

        public:
            //Members
            bool fillVertical; //add rows or columns when grid too small
            uint8 margin;

            //Constructor
            GridLayout();

            //Methods
            Size GetPreferredSize(const WidgetContainer &refContainer);
            void Layout(WidgetContainer &refContainer);
        };

        class ACSTDLIB_API HorizontalLayout : public GridLayout
        {
        public:
            //Constructor
            inline HorizontalLayout()
            {
                this->nRows = 1;
                this->fillVertical = false;
            }
        };

        class ACSTDLIB_API VerticalLayout : public GridLayout
        {
        public:
            //Constructor
            inline VerticalLayout()
            {
                this->nColumns = 1;
                this->fillVertical = true;
            }
        };

		class FormLayout : public GridLayout
		{
		public:
			//Constructor
			inline FormLayout()
			{
				this->nColumns = 2;
				this->fillVertical = true;
			}
		};
    }
}
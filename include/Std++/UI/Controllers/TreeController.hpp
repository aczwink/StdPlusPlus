/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include "../../Definitions.h"
#include "../../Containers/Strings/String.hpp"
#include "ControllerIndex.hpp"

namespace StdXX
{
    namespace UI
    {
		//Forward declarations
		class SelectionController;
		class View;

        class STDPLUSPLUS_API TreeController
        {
            friend class View;
        public:
            //Destructor
            virtual ~TreeController() {}

            //Abstract
            virtual ControllerIndex GetChildIndex(uint32 row, uint32 column, const ControllerIndex &parent = ControllerIndex()) const = 0;
			virtual String GetColumnText(uint32 column) const = 0;
            virtual uint32 GetNumberOfChildren(const ControllerIndex &parent = ControllerIndex()) const = 0;
			/**
			 * A special value is the return value 0. This means that the controller does not provide data in relation with columns.
			 * In other words, the controller contains data but all ControllerIndex instances will have the column field invalid.
			 * This is useful when no column info should be provided in the corresponding views.
			 * For instance a simple ComboBox will have only a list of items without columns and column descriptions.
			 *
			 * @return
			 */
			virtual uint32 GetNumberOfColumns() const = 0;
			virtual ControllerIndex GetParentIndex(const ControllerIndex &index) const = 0;
            virtual String GetText(const ControllerIndex &index) const = 0;

            //Methods
			void ModelChanged();

        protected:
            //Members
            View *view;

			//Inline
			inline ControllerIndex CreateIndex(uint32 row, uint32 column, const void *modelNode) const
			{
				return ControllerIndex(row, column, modelNode, this);
			}

		private:
			//Eventhandlers
			virtual void OnSelectionChanged() const {}
			virtual void OnViewChanged() {}
        };
    }
}
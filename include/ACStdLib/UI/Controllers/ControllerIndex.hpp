/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include "../../Definitions.h"
#include "../../Natural.hpp"

namespace ACStdLib
{
	namespace UI
	{
		//Forward declarations
		class TreeController;

		class ControllerIndex
		{
			friend class TreeController;
		public:
			//Constructor
			inline ControllerIndex() : treeController(nullptr)
			{
				this->row = Natural<uint32>::Max();
				this->column = Natural<uint32>::Max();
				this->modelNode = nullptr;
			}

			//Operators
			inline bool operator==(const ControllerIndex &other) const
			{
				return this->row == other.row && this->column == other.column && this->modelNode == other.modelNode;
			}

			inline bool operator!=(const ControllerIndex &other) const
			{
				return !((*this) == other);
			}

			//Methods
			ControllerIndex GetParent() const;

			//Inline
			inline uint32 GetColumn() const
			{
				return this->column;
			}

			inline uint32 GetRow() const
			{
				return this->row;
			}

			inline bool HasParent() const
			{
				return (*this) != ControllerIndex();
			}

		private:
			//Members
			uint32 row;
			uint32 column;
			/**
			 * The node from the real data model tree.
			 */
			void *modelNode;
			const TreeController *treeController;

			//Constructor
			inline ControllerIndex(uint32 row, uint32 column, void *modelNode, const TreeController *treeController) : treeController(treeController)
			{
				this->row = row;
				this->column = column;
				this->modelNode = modelNode;
			}
		};
	}
}
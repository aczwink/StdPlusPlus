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
#include <ACStdLib/UI/Controllers/ListController.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;

//Public methods
ControllerIndex ListController::GetChildIndex(uint32 row, uint32 column, const ControllerIndex &parent) const
{
	return this->CreateIndex(row, 0, nullptr);
}

OldString ListController::GetColumnText(uint32 column) const
{
	return OldString();
}

uint32 ListController::GetNumberOfChildren(const ControllerIndex &parent) const
{
	if(parent.HasParent())
		return 0; //children (items in list) have no children

	return this->GetNumberOfItems();
}

uint32 ListController::GetNumberOfColumns() const
{
	return 0;
}

OldString ListController::GetText(const ControllerIndex &index) const
{
	return this->GetText(index.GetRow());
}
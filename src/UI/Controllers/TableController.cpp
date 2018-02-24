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
#include <Std++/UI/Controllers/TableController.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::UI;

//Public methods
ControllerIndex TableController::GetChildIndex(uint32 row, uint32 column, const ControllerIndex &parent) const
{
	return this->CreateIndex(row, column, nullptr);
}

uint32 TableController::GetNumberOfChildren(const ControllerIndex &parent) const
{
	if(parent.HasParent())
		return 0; //children (items in list) have no children

	return this->GetNumberOfRows();
}

ControllerIndex TableController::GetParentIndex(const ControllerIndex &index) const
{
	return ControllerIndex(); //flat structure
}

String TableController::GetText(const ControllerIndex &index) const
{
	return this->GetText(index.GetRow(), index.GetColumn());
}
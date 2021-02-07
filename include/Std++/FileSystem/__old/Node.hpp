/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include "Std++/Containers/Strings/String.hpp"
#include "Std++/SmartPointers/AutoPointer.hpp"
#include "Path.hpp"
#include "NodeInfo.hpp"

namespace StdXX::FileSystem
{
	//Forward declarations
	class Directory;
	class RWFileSystem;

	class Node
	{
	public:
		//Abstract
		virtual void ChangePermissions(const FileSystem::NodePermissions& newPermissions) = 0;
		virtual NodeType GetType() const = 0;
		virtual NodeInfo QueryInfo() const = 0;
	};
}
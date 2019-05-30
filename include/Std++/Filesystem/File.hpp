/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#include "../SmartPointers/UniquePointer.hpp"
#include "../Streams/InputStream.hpp"
#include "FileSystemNode.hpp"

namespace StdXX
{
	class File : public FileSystemNode
	{
	public:
		//Abstract
		virtual UniquePointer<InputStream> OpenForReading() const = 0;
		/*
		 * The following special cases aren't implemented yet. They don't need a convenience class like FileOutputStream.
		 * OpenForAppending:
		 * -Create file in case it does not exist
		 * -File pos is at end
		 * -File is of course not truncated
		 *
		 * OpenForUpdating: no mode needed because this would do the following:
		 * -Create file in case it does not exist
		 * -File pos is at beginning
		 * -File is of course not truncated
		 * -File must support streaming
		 *
		 */
		/**
		 * File is truncated.
		 *
		 * @return
		 */
		virtual UniquePointer<OutputStream> OpenForWriting() = 0;

		//Methods
		FileSystemNodeType GetType() const override;
	};
}
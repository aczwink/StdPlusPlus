/*
* Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include "WindowsFile.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
uint64 WindowsFile::GetStoredSize() const
{
	NOT_IMPLEMENTED_ERROR;
	return uint64();
}

uint64 WindowsFile::GetSize() const
{
	NOT_IMPLEMENTED_ERROR;
	return uint64();
}

UniquePointer<InputStream> WindowsFile::OpenForReading() const
{
	NOT_IMPLEMENTED_ERROR;
	return UniquePointer<InputStream>();
}

UniquePointer<OutputStream> WindowsFile::OpenForWriting()
{
	NOT_IMPLEMENTED_ERROR;
	return UniquePointer<OutputStream>();
}

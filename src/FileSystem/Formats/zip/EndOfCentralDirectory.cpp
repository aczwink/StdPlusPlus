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
#include "EndOfCentralDirectory.hpp"
//Local
#include <Std++/Streams/Writers/DataWriter.hpp>
#include "Zip.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Public methods
void EndOfCentralDirectory::Write(OutputStream &outputStream)
{
	DataWriter dataWriter(false, outputStream);

	dataWriter.WriteUInt32(zipEndOfCentralDirectorySignature);
	dataWriter.WriteUInt16(0);
	dataWriter.WriteUInt16(0);
	dataWriter.WriteUInt16(0);
	dataWriter.WriteUInt16(this->nEntries);
	dataWriter.WriteUInt32(0);
	dataWriter.WriteUInt32(this->centralDirectoryOffset);
	dataWriter.WriteUInt16(0);
}
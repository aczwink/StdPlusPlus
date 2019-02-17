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
//Class header
#include <Std++/ChecksumFunction.hpp>
//Local
#include <Std++/Constants.hpp>
#include "CRC32Hasher.hpp"
//Namespaces
using namespace StdXX;

//Public methods
uint64 ChecksumFunction::Update(InputStream &inputStream)
{
	uint64 readSize = 0;
	while(!inputStream.IsAtEnd())
	{
		byte buffer[c_io_blockSize];
		uint32 bytesRead = inputStream.ReadBytes(buffer, sizeof(buffer));
		readSize += bytesRead;
		this->Update(buffer, bytesRead);
	}

	return readSize;
}

//Class functions
UniquePointer<ChecksumFunction> ChecksumFunction::CreateInstance(ChecksumAlgorithm algorithm)
{
	switch (algorithm)
	{
	case ChecksumAlgorithm::CRC32:
		return new _stdxx_::CRC32Hasher;
	}

	return nullptr;
}
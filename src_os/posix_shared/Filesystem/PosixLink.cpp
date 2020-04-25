/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
#include "PosixLink.hpp"
//Global
#include <unistd.h>
//Local
#include "PosixStat.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
Path PosixLink::ReadTarget() const
{
	char buffer[4096];
	ssize_t nBytesInBuffer = readlink(reinterpret_cast<const char *>(this->path.String().ToUTF8().GetRawZeroTerminatedData()), buffer, sizeof(buffer));
	ASSERT(nBytesInBuffer != -1, u8"REPORT THIS PLEASE!");
	ASSERT(nBytesInBuffer < sizeof(buffer), u8"REPORT THIS PLEASE!");

	return String::CopyUtf8Bytes(reinterpret_cast<const uint8 *>(buffer), nBytesInBuffer);
}
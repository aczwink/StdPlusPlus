/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include "gzipCompressor.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Constructor
gzipCompressor::gzipCompressor(OutputStream &outputStream, const Optional<uint8> &compressionLevel) : Compressor(outputStream)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

//Public methods
void _stdxx_::gzipCompressor::Flush()
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

uint32 _stdxx_::gzipCompressor::WriteBytes(const void *source, uint32 size)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return 0;
}
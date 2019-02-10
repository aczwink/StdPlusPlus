/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Compression/Decompressor.hpp>
//Local
#include "LZMA_dec/LZMADecompressor.hpp"
//Namespaces
using namespace StdXX;

//Class functions
Decompressor *Decompressor::Create(CompressionAlgorithm algorithm, InputStream &inputStream)
{
	switch (algorithm)
	{
	case CompressionAlgorithm::LZMA:
		return new _stdxx_::LZMADecompressor(inputStream);
	}
	
	return nullptr;
}

Decompressor *Decompressor::CreateRaw(CompressionAlgorithm algorithm, InputStream &inputStream, const byte* header, uint32 headerSize, uint64 uncompressedSize)
{
	switch (algorithm)
	{
	case CompressionAlgorithm::LZMA:
		return new _stdxx_::LZMADecompressor(inputStream, uncompressedSize, header, headerSize);
	}

	return nullptr;
}
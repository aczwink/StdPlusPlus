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
//Class header
#include <Std++/Compression/Decompressor.hpp>
//Local
#include "ZLIB/ZLIBDecompressor.hpp"
#include "gzip/gzipDecompressor.hpp"
//Extensions
#ifdef _STDXX_EXTENSION_ZLIB
#include "../../src_backends/zlib/ExtZLIBInflater.hpp"
#endif
#ifdef _STDXX_EXTENSION_LIBLZMA
#include "../../src_backends/liblzma/LZMADecompressor.hpp"
#endif
//Namespaces
using namespace StdXX;

//Class functions
Decompressor *Decompressor::Create(CompressionAlgorithm algorithm, InputStream &inputStream)
{
	switch (algorithm)
	{
	case CompressionAlgorithm::DEFLATE:
#ifdef _STDXX_EXTENSION_ZLIB
    return new _stdxx_::ExtZLIBInflater(inputStream);
#endif
		return new _stdxx_::Inflater(inputStream);
	}
	
	return nullptr;
}

Decompressor *Decompressor::Create(CompressionStreamFormatType streamFormatType, InputStream &inputStream, bool verify)
{
	switch(streamFormatType)
	{
		case CompressionStreamFormatType::gzip:
			return new _stdxx_::gzipDecompressor(inputStream, verify);
#ifdef _STDXX_EXTENSION_LIBLZMA
		case CompressionStreamFormatType::lzma:
			return new _stdxx_::LZMADecompressor(inputStream);
#endif
		case CompressionStreamFormatType::zlib:
			return new _stdxx_::ZLIBDecompressor(inputStream, verify);
	}
	return nullptr;
}
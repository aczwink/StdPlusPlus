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
#include <Std++Test.hpp>
using namespace StdXX;

TEST_SUITE(LzmaTest)
{
	TEST(encdec_tests)
	{
		const String data = String(u8"this is a test string...").ToUTF8();
		byte buffer[4096];

		//first compress
		BufferOutputStream bufferOutputStream(buffer, sizeof(buffer));
		UniquePointer<Compressor> compressor = Compressor::Create(CompressionAlgorithm::LZMA, bufferOutputStream);
		compressor->WriteBytes(data.GetRawData(), data.GetSize());
		compressor->Flush();
		uint64 compressedSize = bufferOutputStream.GetCurrentOffset();

		//now decompress again
		BufferInputStream bufferInputStream(buffer, compressedSize);
		UniquePointer<Decompressor> decompressor = Decompressor::Create(CompressionAlgorithm::LZMA, bufferInputStream, false);
		TextReader textReader = TextReader(*decompressor, TextCodecType::UTF8);
		String read = textReader.ReadString(data.GetLength());

		ASSERT(data == read, u8"Expected: '" + data + u8"' Got: " + read);
	}
};
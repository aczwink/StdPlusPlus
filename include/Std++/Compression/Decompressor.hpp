/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/InputStream.hpp>
#include "CompressionAlgorithm.hpp"

namespace StdXX
{
	class STDPLUSPLUS_API Decompressor : public InputStream
	{
	public:
		//Constructor
		inline Decompressor(InputStream &inputStream) : inputStream(inputStream)
		{
		}

		//Destructor
		virtual ~Decompressor()
		{
		}

		//Functions
		static Decompressor *Create(CompressionAlgorithm algorithm, InputStream &inputStream, bool verify);
		static Decompressor *CreateRaw(CompressionAlgorithm algorithm, InputStream &inputStream, const byte* header, uint32 headerSize, uint64 uncompressedSize);

	protected:
		//Members
		InputStream& inputStream;
	};
}
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
//Local
#include <Std++/Compression/Decompressor.hpp>
#include <Std++/Containers/FIFOBuffer.hpp>
#include "LzmaDec.h"

namespace _stdxx_
{
	class LZMADecompressor : public StdXX::Decompressor
	{
	public:
		//Constructors
		LZMADecompressor(InputStream& inputStream);
		LZMADecompressor(InputStream& inputStream, uint64 uncompressedSize, const byte* header, uint32 headerSize);

		//Destructor
		~LZMADecompressor();

		//Methods
		bool IsAtEnd() const override;
		uint32 ReadBytes(void * destination, uint32 count) override;
		uint32 Skip(uint32 nBytes) override;

	private:
		//Members
		CLzmaDec state;
		uint64 uncompressedSize;
		uint64 leftSize;
		StdXX::FIFOBuffer buffer;

		//Methods
		void Decode(bool write, ELzmaFinishMode finishMode = LZMA_FINISH_ANY);

		inline bool IsUncompressedSizeKnown() const
		{
			return this->uncompressedSize != StdXX::Unsigned<uint64>::Max();
		}
	};
}
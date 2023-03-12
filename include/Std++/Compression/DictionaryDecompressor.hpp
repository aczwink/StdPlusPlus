/*
 * Copyright (c) 2019-2023 Amir Czwink (amir130@hotmail.de)
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
#include "Decompressor.hpp"
#include "SlidingDictionary.hpp"

namespace StdXX
{
	class DictionaryDecompressor : public Decompressor
	{
	public:
		//Constructor
		inline DictionaryDecompressor(InputStream& inputStream, uint32 dictSize) : Decompressor(inputStream), dictionary(dictSize)
		{
			this->nBytesInBuffer = 0;
		}

		uint32 GetBytesAvailable() const override;
		bool IsAtEnd() const override;
		uint32 ReadBytes(void *destination, uint32 count) override;

	private:
		//State
		uint16 nBytesInBuffer;
		SlidingDictionary dictionary;

		//Abstract
		virtual uint32 DecompressNextBlock(SlidingDictionary& dictionary) = 0;

		//Inline
		inline bool IsBufferEmpty() const
		{
			return this->nBytesInBuffer == 0;
		}

		inline void FillBufferIfEmpty()
		{
			if (this->IsBufferEmpty() and !this->inputStream.IsAtEnd())
				this->nBytesInBuffer += this->DecompressNextBlock(this->dictionary);
		}
	};
}
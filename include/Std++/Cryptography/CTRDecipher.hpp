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
#pragma once
//Local
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/Streams/InputStream.hpp>
#include "BlockCipher.hpp"
#include "Counter.hpp"

namespace StdXX
{
	namespace Crypto
	{
		class STDPLUSPLUS_API CTRDecipher : public InputStream
		{
		public:
			//Constructor
			inline CTRDecipher(CipherAlgorithm algorithm, const uint8* key, uint16 keyLength, Counter& counter, InputStream& inputStream) :
					cipher(BlockCipher::Create(algorithm, key, keyLength * 8_u16)), counter(counter), inputStream(inputStream),
			buffer(cipher->GetBlockSize()), leftBytesInBlock(0)
			{
			}

			//Methods
			uint32 GetBytesAvailable() const override;
			bool IsAtEnd() const override;
			uint32 ReadBytes(void *destination, uint32 count) override;
			uint32 Skip(uint32 nBytes) override;

		private:
			//Members
			UniquePointer<BlockCipher> cipher;
			Counter& counter;
			InputStream& inputStream;
			FixedArray<uint8> buffer;
			uint8 leftBytesInBlock;
		};
	}
}
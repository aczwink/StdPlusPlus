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
#include <Std++/Streams/OutputStream.hpp>
#include "CipherAlgorithm.hpp"
#include "Counter.hpp"
#include "BlockCipher.hpp"

namespace StdXX
{
	namespace Crypto
	{
		class STDPLUSPLUS_API CTRCipher : public OutputStream
		{
		public:
			//Constructor
			inline CTRCipher(CipherAlgorithm algorithm, const uint8* key, uint16 keyLength, Counter& counter, OutputStream& outputStream)
					: cipher(BlockCipher::Create(algorithm, key, keyLength)), counter(counter), outputStream(outputStream),
					  buffer(cipher->GetBlockSize()), leftBytesInBlock(0)
			{
				ASSERT(this->counter.GetKeyStreamBlockSize() == this->cipher->GetBlockSize(), u8"Keystream block size must be equal to cipher block size.");
			}

			//Methods
			void Flush() override;
			uint32 WriteBytes(const void *source, uint32 size) override;

		private:
			//Members
			UniquePointer<BlockCipher> cipher;
			Counter& counter;
			OutputStream& outputStream;
			FixedArray<uint8> buffer;
			uint8 leftBytesInBlock;
		};
	}
}
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
#include "BlockCipher.hpp"

namespace StdXX
{
	namespace Crypto
	{
		class CBCCipher : public OutputStream
		{
		public:
			//Constructor
			inline CBCCipher(CipherAlgorithm algorithm, const uint8* key, uint16 keyLength, const uint8* initializationVector, OutputStream& outputStream)
					: cipher(BlockCipher::Create(algorithm, key, keyLength * 8_u16)), outputStream(outputStream), unencrypted(cipher->GetBlockSize()),
					  encryptedBuf1(cipher->GetBlockSize()), encryptedBuf2(cipher->GetBlockSize()), nBytesInBlock(0)
			{
				this->currentBlock = &this->encryptedBuf1[0];
				this->lastBlock = &this->encryptedBuf2[0];
				MemCopy(this->lastBlock, initializationVector, this->cipher->GetBlockSize());
			}

			//Methods
			void Finalize();
			void Flush() override;
			uint32 WriteBytes(const void *source, uint32 size) override;

		private:
			//Members
			UniquePointer<BlockCipher> cipher;
			OutputStream& outputStream;
			FixedArray<uint8> unencrypted;
			FixedArray<uint8> encryptedBuf1;
			FixedArray<uint8> encryptedBuf2;
			uint8* currentBlock;
			uint8* lastBlock;
			uint8 nBytesInBlock;

			//Methods
			void FlushBufferIfFull();
		};
	}
}
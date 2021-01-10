/*
* Copyright (c) 2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/InputStream.hpp>
#include <Std++/Cryptography/CTRDecipher.hpp>
#include <Std++/Cryptography/DefaultCounter.hpp>
#include <Std++/Streams/BufferOutputStream.hpp>
#include <Std++/Streams/Writers/DataWriter.hpp>
#include <Std++/Cryptography/HashFunction.hpp>
#include <Std++/Cryptography/HashingInputStream.hpp>

namespace _stdxx_
{
	/*
	 * Implementation based on:
	 * https://www.winzip.com/win/en/aes_info.html
	 * A Password Based File Encryption Utility by Dr. Gladman (http://brg.a2hosted.com//oldsite/cryptography_technology/fileencrypt/index.php)
	 * */

	//dr. gladmans code increments a 16 byte buffer from low to high address (i.e. little endian)
	class AECounter : public StdXX::Crypto::Counter
	{
	public:
		//Constructor
		inline AECounter()
		{
			StdXX::MemZero(this->keyStreamBlock, sizeof(this->keyStreamBlock));
			//dr. gladmans code initializes the counter to zero, however he increments !before! encrypting the keystream block
			this->keyStreamBlock[0] = 1; //we init it to one since CTRDecipher-class increments !after! encryption
		}

		inline const void *GetKeyStreamBlock() const override
		{
			return this->keyStreamBlock;
		}

		inline uint8 GetKeyStreamBlockSize() const override
		{
			return 16;
		}

		inline void Increment() override
		{
			for(uint8 i = 0; i < 8; i++)
			{
				if(++this->keyStreamBlock[i] != 0)
					break;
			}
		}

	private:
		//Members
		uint8 keyStreamBlock[16];
	};

	class AEDecrypter : public StdXX::InputStream
	{
	public:
		//Constructor
		AEDecrypter(uint32 compressedSize, uint8 strength, const StdXX::String& password, StdXX::InputStream& inputStream, bool verify);

		uint32 GetBytesAvailable() const override;
		bool IsAtEnd() const override;
		uint32 ReadBytes(void *destination, uint32 count) override;
		uint32 Skip(uint32 nBytes) override;

	private:
		//Members
		uint32 leftCompressedSize;
		StdXX::UniquePointer<StdXX::Crypto::CTRDecipher> decipher;
		StdXX::UniquePointer<StdXX::Crypto::HashFunction> authenticator;
		StdXX::UniquePointer<StdXX::Crypto::HashingInputStream> authenticatorStream;
		StdXX::InputStream& baseInputStream;
		AECounter counter;
	};
}
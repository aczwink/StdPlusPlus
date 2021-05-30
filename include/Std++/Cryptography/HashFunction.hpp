/*
 * Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Buffers/FixedSizeBuffer.hpp>
#include <Std++/Containers/Strings/ConstStringIterator.hpp>
#include <Std++/Mathematics.hpp>

namespace StdXX::Crypto
{
	enum class HashAlgorithm
	{
		MD5,
		SHA1,
		SHA256,
		SHA512,
		SHA512_256
	};

	class STDPLUSPLUS_API HashFunction
	{
	public:
		//Destructor
		virtual ~HashFunction()
		{}

		//Abstract
		/**
		 * The size of the underlying block size in bytes.
		 * @return
		 */
		virtual uint32 GetBlockSize() const = 0;
		/**
		 * The size of the digest in bytes.
		 * @return
		 */
		virtual uint32 GetDigestSize() const = 0;
		virtual void Finish() = 0;
		virtual void StoreDigest(void* target) const = 0;
		virtual void Update(const void* buffer, uint32 size) = 0;

		//Methods
		uint64 Update(InputStream& inputStream);

		//Functions
		static UniquePointer<HashFunction> CreateInstance(HashAlgorithm algorithm);

		inline static FixedSizeBuffer HexStringToBytes(const String& string)
		{
			FixedSizeBuffer buffer(string.GetLength()/2);

			auto it = string.begin();
			for(uint32 i = 0; i < buffer.Size(); i++)
			{
				buffer[i] = FromHexDigit(*it) << 4;
				++it;
				buffer[i] |= FromHexDigit(*it);
				++it;
			}
			return buffer;
		}

		//Inline
		inline FixedSizeBuffer GetDigest() const
		{
			FixedSizeBuffer digest(this->GetDigestSize());
			this->StoreDigest(digest.Data());
			return digest;
		}

		inline String GetDigestString() const
		{
			String result;
			FixedSizeBuffer bytes = this->GetDigest();
			for(uint32 i = 0; i < bytes.Size(); i++)
				result += String::HexNumber(bytes.Data()[i], 2, false);
			return result;
		}

	private:
		//Functions
		inline static uint8 FromHexDigit(uint32 codePoint)
		{
			if(Math::IsValueInInterval(char8_t(codePoint), u8'0', u8'9'))
				return codePoint - u8'0';
			return codePoint + 10 - u8'a';
		}
	};
}
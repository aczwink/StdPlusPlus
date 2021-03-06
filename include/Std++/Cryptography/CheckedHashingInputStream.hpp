/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/ReadOnlyInputStream.hpp>
#include "HashFunction.hpp"

namespace StdXX::Crypto
{
	class CheckedHashingInputStream : public ReadOnlyInputStream
	{
	public:
		//Constructors
		inline CheckedHashingInputStream(InputStream &inputStream, HashAlgorithm algorithm, const String& expectedHash)
			: inputStream(inputStream), expectedHash(HashFunction::HexStringToBytes(expectedHash))
		{
			this->hasher = HashFunction::CreateInstance(algorithm);
			this->finished = false;
		}

		inline CheckedHashingInputStream(InputStream &inputStream, HashAlgorithm algorithm, const FixedSizeBuffer& expectedHash)
			: inputStream(inputStream), expectedHash(expectedHash)
		{
			this->hasher = HashFunction::CreateInstance(algorithm);
			this->finished = false;
		}

		//Methods
		uint32 GetBytesAvailable() const override;
		bool IsAtEnd() const override;
		uint32 ReadBytes(void *destination, uint32 count) override;

	private:
		//Members
		InputStream& inputStream;
		UniquePointer<HashFunction> hasher;
		FixedSizeBuffer expectedHash;
		bool finished;

		//Methods
		void Finish();
	};
}
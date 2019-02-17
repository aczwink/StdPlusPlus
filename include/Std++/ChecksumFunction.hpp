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
#pragma once
 //Local
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Streams/InputStream.hpp>
#include "Definitions.h"

namespace StdXX
{
	enum ChecksumAlgorithm
	{
		CRC32,
	};

	class ChecksumFunction
	{
	public:
		//Destructor
		virtual ~ChecksumFunction(){}

		//Abstract
		virtual uint32 GetChecksumSize() const = 0;
		virtual void Finish() = 0;
		virtual void StoreChecksum(void* target) const = 0;
		virtual void Update(const void* buffer, uint32 size) = 0;

		//Methods
		uint64 Update(InputStream& inputStream);

		//Functions
		static UniquePointer<ChecksumFunction> CreateInstance(ChecksumAlgorithm algorithm);

		//Inline
		inline FixedArray<byte> GetChecksum() const
		{
			FixedArray<byte> digest(this->GetChecksumSize());
			this->StoreChecksum(&digest[0]);
			return digest;
		}

		inline String GetChecksumString() const
		{
			String result;
			FixedArray<byte> bytes = this->GetChecksum();
			for(byte b : bytes)
				result += String::HexNumber(b, 2, false);
			return result;
		}
	};
}
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
#include <Std++/Containers/Array/DynamicArray.hpp>
#include <Std++/Cryptography/Counter.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/Utility.hpp>
#include "InputStream.hpp"

namespace StdXX
{
	class ChainedInputStream : public InputStream
	{
	public:
		//Constructor
		inline ChainedInputStream(UniquePointer <InputStream> &&base)
		{
			this->ownedStreams.Push(Move(base));
			this->end = this->ownedStreams.Last().operator->();
		}

		//Methods
		uint32 GetBytesAvailable() const override;
		bool IsAtEnd() const override;
		uint32 ReadBytes(void *destination, uint32 count) override;
		uint32 Skip(uint32 nBytes) override;

		//Inline
		inline void Add(UniquePointer <InputStream> &&inputStream)
		{
			this->ownedStreams.Push(Move(inputStream));
			this->end = this->ownedStreams.Last().operator->();
		}

		inline Crypto::Counter* AddCounter(UniquePointer<Crypto::Counter>&& counter)
		{
			this->ownedCounters.Push(Move(counter));
			return this->ownedCounters.Last().operator->();
		}

		inline InputStream& GetEnd()
		{
			return *this->end;
		}

	private:
		//Members
		InputStream* end;
		DynamicArray<UniquePointer<InputStream>> ownedStreams;
		DynamicArray<UniquePointer<Crypto::Counter>> ownedCounters;
	};
}
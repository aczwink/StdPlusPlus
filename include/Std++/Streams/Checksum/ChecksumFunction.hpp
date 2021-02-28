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
#include "Std++/Definitions.h"

namespace StdXX
{
	enum ChecksumAlgorithm
	{
		ADLER32,
		CRC32,
	};

	class ChecksumFunction
	{
	public:
		//Destructor
		virtual ~ChecksumFunction(){}

		//Abstract
		virtual void Finish() = 0;
		virtual uint64 GetChecksum() const = 0;
        virtual uint8 GetChecksumSize() const = 0;
		virtual void Update(const void* buffer, uint32 size) = 0;

		//Methods
		uint64 Update(InputStream& inputStream);

		//Functions
		static UniquePointer<ChecksumFunction> CreateInstance(ChecksumAlgorithm algorithm);
	};
}
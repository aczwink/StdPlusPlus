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
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/ChecksumFunction.hpp>
#include "OutputStream.hpp"

namespace StdXX
{
	/**
	 * See HashingInputStream. This is basically the same, with the saddle difference that this stream pipes
	 * to an outputstream.
	 */
	class STDPLUSPLUS_API HashingOutputStream : public OutputStream
	{
	public:
		//Constructor
		inline HashingOutputStream(OutputStream& outputStream, ChecksumAlgorithm algorithm) : outputStream(outputStream), algorithm(algorithm)
		{
			this->hasher = ChecksumFunction::CreateInstance(this->algorithm);
		}

		//Methods
		void Flush() override;
		uint32 WriteBytes(const void *source, uint32 size) override;

		//Inline
		inline UniquePointer<ChecksumFunction> Reset()
		{
			UniquePointer<ChecksumFunction> tmp = Move(this->hasher);
			this->hasher = ChecksumFunction::CreateInstance(this->algorithm);

			return tmp;
		}

	private:
		//Members
		OutputStream& outputStream;
		ChecksumAlgorithm algorithm;
		UniquePointer<ChecksumFunction> hasher;
	};
}
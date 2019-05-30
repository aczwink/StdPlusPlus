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
//Local
#include "ChecksumFunction.hpp"
#include "InputStream.hpp"

namespace StdXX
{
	class STDPLUSPLUS_API CheckedInputStream : public InputStream
	{
	public:
		//Constructor
		inline CheckedInputStream(InputStream &inputStream, ChecksumAlgorithm algorithm) : inputStream(inputStream), algorithm(algorithm)
		{
			this->checkFunc = ChecksumFunction::CreateInstance(this->algorithm);
		}

		//Methods
		uint32 GetBytesAvailable() const override;
		bool IsAtEnd() const override;
		uint32 ReadBytes(void *destination, uint32 count) override;
		uint32 Skip(uint32 nBytes) override;

		//Inline
		inline UniquePointer<ChecksumFunction> Reset()
		{
			UniquePointer<ChecksumFunction> tmp = Move(this->checkFunc);
			this->checkFunc = ChecksumFunction::CreateInstance(this->algorithm);

			return tmp;
		}

	private:
		//Members
		InputStream &inputStream;
		ChecksumAlgorithm algorithm;
		UniquePointer<ChecksumFunction> checkFunc;
	};
}
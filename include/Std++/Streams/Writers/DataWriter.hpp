/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "../OutputStream.hpp"

namespace StdPlusPlus
{
	class DataWriter
	{
	public:
		//Constructor
		inline DataWriter(bool writeBigEndian, OutputStream &outputStream) : outputStream(outputStream), writeBigEndian(writeBigEndian)
		{
		}

		//Operators
		inline DataWriter &operator<<(char c)
		{
			this->WriteByte(c);
			return *this;
		}

		inline DataWriter &operator<<(uint32 i)
		{
			this->WriteUInt32(i);
			return *this;
		}

		//Inline
		inline void WriteByte(byte b)
		{
			this->outputStream.WriteBytes(&b, 1);
		}

		inline void WriteInt16(int16 value)
		{
			if(this->writeBigEndian)
				this->WriteInt16BE(value);
			else
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}

		inline void WriteUInt32(uint32 value)
		{
			if(this->writeBigEndian)
				this->WriteUInt32BE(value);
			else
				this->WriteUInt32LE(value);
		}

	private:
		//Members
		bool writeBigEndian;
		OutputStream &outputStream;

		//Inline
#ifdef _STDPP_ENDIAN_LITTLE
		inline void WriteInt16BE(int16 value)
		{
			byte b[2];

			b[0] = (uint8)(value >> 8);
			b[1] = (uint8)(value & 0xFF);

			this->outputStream.WriteBytes(&b, sizeof(b));
		}

		inline void WriteUInt32BE(uint32 value)
		{
			byte b[4];

			b[0] = (uint8)(value >> 24);
			b[1] = (uint8)((value >> 16) & 0xFF);
			b[2] = (uint8)((value >> 8) & 0xFF);
			b[3] = (uint8)(value & 0xFF);

			this->outputStream.WriteBytes(&b, sizeof(b));
		}

		inline void WriteUInt32LE(uint32 value)
		{
			this->outputStream.WriteBytes(&value, sizeof(value));
		}
#endif
	};
}
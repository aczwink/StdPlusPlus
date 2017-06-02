/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "ACStdLib/Streams/InputStream.hpp"

namespace ACStdLib
{
    class ACSTDLIB_API DataReader
    {
    private:
        //Members
        bool readBigEndian;
		InputStream &inputStream;

		//Inline
#ifdef _AC_ENDIAN_LITTLE
		inline float32 ReadFloat32BE()
		{
			union
			{
				uint32 i;
				float32 f;
			};

			i = this->ReadUInt32BE();

			return f;
		}

		inline float32 ReadFloat32LE()
		{
			float32 v;

			this->inputStream.ReadBytes(&v, sizeof(v));

			return v;
		}

		inline float64 ReadFloat64BE()
		{
			union
			{
				uint64 i;
				float64 f;
			};

			i = this->ReadUInt64BE();

			return f;
		}

		inline int16 ReadInt16BE()
		{
			return this->ReadUInt16BE();
		}

		inline int16 ReadInt16LE()
		{
			int16 v;

			this->inputStream.ReadBytes(&v, sizeof(v));

			return v;
		}

		inline int32 ReadInt32LE()
		{
			int32 v;

			this->inputStream.ReadBytes(&v, sizeof(v));

			return v;
		}

		inline int64 ReadInt64LE()
		{
			int64 v;

			this->inputStream.ReadBytes(&v, sizeof(v));

			return v;
		}

		inline uint16 ReadUInt16BE()
		{
			byte buffer[2];

			this->inputStream.ReadBytes(&buffer, sizeof(buffer));

			return ((uint16)buffer[0] << 8) | buffer[1];
		}

		inline uint16 ReadUInt16LE()
		{
			uint16 v;

			this->inputStream.ReadBytes(&v, sizeof(v));

			return v;
		}

		inline uint32 ReadUInt32BE()
		{
			byte buffer[4];

			this->inputStream.ReadBytes(&buffer, sizeof(buffer));

			return ((uint32)buffer[0] << 24) | ((uint32)buffer[1] << 16) | ((uint32)buffer[2] << 8) | buffer[3];
		}

		inline uint32 ReadUInt32LE()
		{
			uint32 v;

			this->inputStream.ReadBytes(&v, sizeof(v));

			return v;
		}

		inline uint64 ReadUInt64BE()
		{
			byte buffer[8];

			this->inputStream.ReadBytes(&buffer, sizeof(buffer));

			return ((uint64)buffer[0] << 56) | ((uint64)buffer[1] << 48) | ((uint64)buffer[2] << 40) | ((uint64)buffer[3] << 32) | ((uint64)buffer[4] << 24) | ((uint64)buffer[5] << 16) | ((uint64)buffer[6] << 8) | buffer[7];
		}

		inline uint64 ReadUInt64LE()
		{
			uint64 v;

			this->inputStream.ReadBytes(&v, sizeof(v));

			return v;
		}
#endif

    public:
        //Constructor
        inline DataReader(bool readBigEndian, InputStream &inputStream) : inputStream(inputStream)
        {
            this->readBigEndian = readBigEndian;
        }

		//Operators
		inline DataReader &operator>>(char &c)
		{
			this->inputStream.ReadBytes(&c, 1);

			return *this;
		}

		inline DataReader &operator>>(byte &b)
		{
			this->inputStream.ReadBytes(&b, 1);

			return *this;
		}

		inline DataReader &operator>>(int16 &i)
		{
			if(this->readBigEndian)
				i = this->ReadInt16BE();
			else
				i = this->ReadInt16LE();

			return *this;
		}

		inline DataReader &operator>>(uint16 &i)
		{
			if(this->readBigEndian)
				i = this->ReadUInt16BE();
			else
				i = this->ReadUInt16LE();

			return *this;
		}

		inline DataReader &operator>>(uint32 &i)
		{
			if(this->readBigEndian)
				i = this->ReadUInt32BE();
			else
				i = this->ReadUInt32LE();

			return *this;
		}
    };
}
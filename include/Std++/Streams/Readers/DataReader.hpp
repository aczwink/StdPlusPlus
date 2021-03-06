/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#include "../InputStream.hpp"

namespace StdXX
{
    class STDPLUSPLUS_API DataReader
    {
	public:
		//Constructor
		inline DataReader(bool readBigEndian, InputStream &inputStream) : inputStream(inputStream)
		{
			this->readBigEndian = readBigEndian;
		}

		//Operators
		inline DataReader &operator>>(char &c)
		{
			uint32 nBytesRead = this->inputStream.ReadBytes(&c, 1);
			ASSERT_EQUALS(1, nBytesRead);

			return *this;
		}

		inline DataReader &operator>>(byte &b)
		{
			uint32 nBytesRead = this->inputStream.ReadBytes(&b, 1);
			ASSERT_EQUALS(1, nBytesRead);

			return *this;
		}

		inline DataReader &operator>>(int16 &i)
		{
			i = this->ReadInt16();

			return *this;
		}

		inline DataReader &operator>>(uint16 &i)
		{
			i = this->ReadUInt16();

			return *this;
		}

		inline DataReader &operator>>(uint32 &i)
		{
			i = this->ReadUInt32();

			return *this;
		}

		//Properties
		inline class InputStream& InputStream()
		{
			return this->inputStream;
		}

		//Inline
		inline byte ReadByte()
		{
			byte b;

			uint32 nBytesRead = this->inputStream.ReadBytes(&b, 1);
			ASSERT_EQUALS(1, nBytesRead);

			return b;
		}

		inline void ReadBytes(void* destination, uint32 nBytes)
		{
			uint32 nBytesRead = this->inputStream.ReadBytes(destination, nBytes);
			ASSERT_EQUALS(nBytes, nBytesRead);
		}

		inline float32 ReadFloat32()
		{
			float32 v;

			if(this->readBigEndian)
			{
#ifdef XPC_ENDIANNESS_LITTLE
				union
				{
					uint32 i;
					float32 f;
				};

				i = this->ReadUInt32BE();

				return f;
#endif
			}

#ifdef XPC_ENDIANNESS_LITTLE
			uint32 nBytesRead = this->inputStream.ReadBytes(&v, sizeof(v));
			ASSERT_EQUALS(sizeof(v), nBytesRead);
#endif
			return v;
		}

		inline float64 ReadFloat64()
		{
			float64 v;

			if(this->readBigEndian)
			{
#ifdef XPC_ENDIANNESS_LITTLE
				union
				{
					uint64 i;
					float64 f;
				};

				i = this->ReadUInt64BE();

				return f;
#endif
			}
#ifdef XPC_ENDIANNESS_LITTLE
			uint32 nBytesRead = this->inputStream.ReadBytes(&v, sizeof(v));
			ASSERT_EQUALS(sizeof(v), nBytesRead);
#endif

			return v;
		}

		inline int8 ReadInt8()
		{
			return this->ReadByte();
		}

		inline int16 ReadInt16()
		{
			return this->ReadUInt16();
		}

		inline int32 ReadInt32()
		{
			return this->ReadUInt32();
		}

		inline uint16 ReadUInt16()
		{
			uint16 v;

			uint32 nBytesRead = this->inputStream.ReadBytes(&v, sizeof(v));
			ASSERT_EQUALS(sizeof(v), nBytesRead);

			if(this->readBigEndian)
#ifdef XPC_ENDIANNESS_LITTLE
				return this->Swap16(v);
#else
			return v;
#endif

#ifdef XPC_ENDIANNESS_LITTLE
			return v;
#else
			return this->Swap16(v);
#endif
		}

		inline uint32 ReadUInt32()
		{
			if(this->readBigEndian)
				return this->ReadUInt32BE();

			return this->ReadUInt32LE();
		}

		inline uint64 ReadUInt64()
		{
			if(this->readBigEndian)
				return this->ReadUInt64BE();
			return this->ReadUInt64LE();
		}

		//Inline
		inline uint32 Skip(uint32 nBytes)
		{
			return this->inputStream.Skip(nBytes);
		}

    private:
        //Members
        bool readBigEndian;
		class InputStream &inputStream;

		//Inline
		inline uint16 Swap16(uint16 v)
		{
			return (((uint16)(v & 0xFF) << 8) | ((uint16)(v & 0xFF00) >> 8));
		}

#ifdef XPC_ENDIANNESS_LITTLE
		inline int64 ReadInt64LE()
		{
			int64 v;

			uint32 nBytesRead = this->inputStream.ReadBytes(&v, sizeof(v));
			ASSERT_EQUALS(sizeof(v), nBytesRead);

			return v;
		}

		inline uint32 ReadUInt32BE()
		{
			byte buffer[4];

			uint32 nBytesRead = this->inputStream.ReadBytes(&buffer, sizeof(buffer));
			ASSERT_EQUALS(sizeof(buffer), nBytesRead);

			return ((uint32)buffer[0] << 24) | ((uint32)buffer[1] << 16) | ((uint32)buffer[2] << 8) | buffer[3];
		}

		inline uint32 ReadUInt32LE()
		{
			uint32 v;

			uint32 nBytesRead = this->inputStream.ReadBytes(&v, sizeof(v));
			ASSERT_EQUALS(sizeof(v), nBytesRead);

			return v;
		}

		inline uint64 ReadUInt64BE()
		{
			byte buffer[8];

			uint32 nBytesRead = this->inputStream.ReadBytes(&buffer, sizeof(buffer));
			ASSERT_EQUALS(sizeof(buffer), nBytesRead);

			return ((uint64)buffer[0] << 56) | ((uint64)buffer[1] << 48) | ((uint64)buffer[2] << 40) | ((uint64)buffer[3] << 32) | ((uint64)buffer[4] << 24) | ((uint64)buffer[5] << 16) | ((uint64)buffer[6] << 8) | buffer[7];
		}

		inline uint64 ReadUInt64LE()
		{
			uint64 v;

			uint32 nBytesRead = this->inputStream.ReadBytes(&v, sizeof(v));
			ASSERT_EQUALS(sizeof(v), nBytesRead);

			return v;
		}
#endif
    };
}
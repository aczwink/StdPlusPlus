/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Definitions.h>

namespace StdXX
{
    class STDPLUSPLUS_API OutputStream
    {
    public:
        //Destructor
        virtual ~OutputStream(){};

        /*
        //Inline Operators
        inline OutputStream &operator<<(int64 i)
        {
            const OldString &refString = ToString(i);

            return *this << refString;
        }

        inline OutputStream &operator<<(const ByteString &refString)
        {
            this->WriteBytes(refString.GetC_Str(), refString.GetNumberOfElements());

            return *this;
        }

        inline OutputStream &operator<<(const UTF8String &refString)
        {
            this->WriteBytes(refString.GetC_Str(), refString.GetNumberOfElements());

            return *this;
        }

        inline OutputStream &operator<<(const UTF16String &refString)
        {
            return *this << UTF8String(refString);
        }

        inline OutputStream &operator<<(const UTF32String &refString)
        {
            return *this << UTF8String(refString);
        }
         */

        //Abstract
		virtual void Flush() = 0;
        virtual uint32 WriteBytes(const void *source, uint32 size) = 0;

        //Inline
#ifdef XPC_ENDIANNESS_LITTLE
        inline void WriteFloat32LE(float32 value)
		{
			this->WriteBytes(&value, sizeof(value));
		}

		inline void WriteInt32LE(int32 value)
		{
			this->WriteBytes(&value, sizeof(value));
		}

		inline void WriteInt64LE(int64 value)
		{
			this->WriteBytes(&value, sizeof(value));
		}
#endif
        /*
        inline void WriteInt32LE(int32 value)
        {
            byte b[4];

            b[0] = (uint8)(value & 0xFF);
            b[1] = (uint8)((value >> 8) & 0xFF);
            b[2] = (uint8)((value >> 16) & 0xFF);
            b[3] = (uint8)(value >> 24);

            this->WriteBytes(&b, sizeof(b));
        }

        inline void WriteInt64LE(int64 value)
        {
            byte b[8];

            b[0] = (uint8)(value & 0xFF);
            b[1] = (uint8)((value >> 8) & 0xFF);
            b[2] = (uint8)((value >> 16) & 0xFF);
            b[3] = (uint8)((value >> 24) & 0xFF);
            b[4] = (uint8)((value >> 32) & 0xFF);
            b[5] = (uint8)((value >> 40) & 0xFF);
            b[6] = (uint8)((value >> 48) & 0xFF);
            b[7] = (uint8)((value >> 56));

            this->WriteBytes(&b, sizeof(b));
        }

        inline void WriteUInt16LE(uint16 value)
        {
            byte b[2];

            b[0] = (uint8)(value & 0xFF);
            b[1] = (uint8)((value >> 8) & 0xFF);

            this->WriteBytes(&b, sizeof(b));
        }

        inline void WriteUInt64LE(uint64 value)
        {
            byte b[8];

            b[0] = (uint8)(value & 0xFF);
            b[1] = (uint8)((value >> 8) & 0xFF);
            b[2] = (uint8)((value >> 16) & 0xFF);
            b[3] = (uint8)((value >> 24) & 0xFF);
            b[4] = (uint8)((value >> 32) & 0xFF);
            b[5] = (uint8)((value >> 40) & 0xFF);
            b[6] = (uint8)((value >> 48) & 0xFF);
            b[7] = (uint8)((value >> 56));

            this->WriteBytes(&b, sizeof(b));
        }
        */
    };
}
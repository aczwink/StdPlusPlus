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
#include "../Definitions.h"
#include "ACStdLib/Containers/Strings/String.h"
#include "ACStdLib/Containers/Strings/StringUtil.h"
#include "ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp"
#include "ACStdLib/Filesystem/Path.hpp"
#include "../InternalCompilerFlags.h"

namespace ACStdLib
{
    class CLineBreak{};
    static const CLineBreak endl;

    class ACSTDLIB_API AOutputStream
    {
    public:
        //Destructor
        virtual ~AOutputStream(){};

        //Inline Operators
        inline AOutputStream &operator<<(char c)
        {
            this->WriteByte(c);

            return *this;
        }

        inline AOutputStream &operator<<(const char *pString)
        {
            this->WriteBytes(pString, GetStringLength(pString));

            return *this;
        }

        inline AOutputStream &operator<<(uint16 i)
        {
            const String &refString = ToString((uint64)i);

            return *this << refString;
        }

        inline AOutputStream &operator<<(int32 i)
        {
            const String &refString = ToString((int64)i);

            return *this << refString;
        }

        inline AOutputStream &operator<<(uint32 i)
        {
            const String &refString = ToString((uint64)i);

            return *this << refString;
        }

        inline AOutputStream &operator<<(int64 i)
        {
            const String &refString = ToString(i);

            return *this << refString;
        }

        inline AOutputStream &operator<<(uint64 i)
        {
            const String &refString = ToString(i);

            return *this << refString;
        }

        inline AOutputStream &operator<<(float64 f)
        {
            const String &refString = ToString(f);

            return *this << refString;
        }

        inline AOutputStream &operator<<(const CLineBreak &refLineBreak)
        {
            this->WriteBytes("\r\n", 2); //do always CR LF.. ok for windows and linux ignores CR

            return *this;
        }

        inline AOutputStream &operator<<(const String &refString)
        {
            return *this << refString.GetUTF16();
        }

        inline AOutputStream &operator<<(const C8BitString &refString)
        {
            this->WriteBytes(refString.GetC_Str(), refString.GetNumberOfElements());

            return *this;
        }

        inline AOutputStream &operator<<(const UTF8String &refString)
        {
            this->WriteBytes(refString.GetC_Str(), refString.GetNumberOfElements());

            return *this;
        }

        inline AOutputStream &operator<<(const CUTF16String &refString)
        {
            return *this << UTF8String(refString);
        }

        inline AOutputStream &operator<<(const CUTF32String &refString)
        {
            return *this << UTF8String(refString);
        }

        inline AOutputStream &operator<<(const Path &refPath)
        {
            return *this << refPath.GetString();
        }

        //Abstract
        virtual void WriteByte(byte b) = 0;
        virtual uint32 WriteBytes(const void *pSource, uint32 size) = 0;

        //Inline
#ifdef _AC_ENDIAN_LITTLE
        inline void WriteFloat32LE(float32 value)
		{
			this->WriteBytes(&value, sizeof(value));
		}

		inline void WriteFloat64BE(float64 value)
		{
			union
			{
				uint64 i;
				float64 f;
			};

			f = value;
			this->WriteUInt64BE(i);
		}

		inline void WriteFloat64LE(float64 value)
		{
			this->WriteBytes(&value, sizeof(value));
		}

		inline void WriteInt16BE(int16 value)
		{
			byte b[2];

			b[0] = (uint8)(value >> 8);
			b[1] = (uint8)(value & 0xFF);

			this->WriteBytes(&b, sizeof(b));
		}

		inline void WriteInt32LE(int32 value)
		{
			this->WriteBytes(&value, sizeof(value));
		}

		inline void WriteInt64LE(int64 value)
		{
			this->WriteBytes(&value, sizeof(value));
		}

		inline void WriteUInt16BE(uint16 value)
		{
			byte b[2];

			b[0] = (uint8)(value >> 8);
			b[1] = (uint8)(value & 0xFF);

			this->WriteBytes(&b, sizeof(b));
		}

		inline void WriteUInt16LE(uint16 value)
		{
			this->WriteBytes(&value, sizeof(value));
		}

		inline void WriteUInt32BE(uint32 value)
		{
			byte b[4];

			b[0] = (uint8)(value >> 24);
			b[1] = (uint8)((value >> 16) & 0xFF);
			b[2] = (uint8)((value >> 8) & 0xFF);
			b[3] = (uint8)(value & 0xFF);

			this->WriteBytes(&b, sizeof(b));
		}

		inline void WriteUInt32LE(uint32 value)
		{
			this->WriteBytes(&value, sizeof(value));
		}

		inline void WriteUInt64BE(uint64 value)
		{
			byte b[8];

			b[0] = (uint8)((value >> 56));
			b[1] = (uint8)((value >> 48) & 0xFF);
			b[2] = (uint8)((value >> 40) & 0xFF);
			b[3] = (uint8)((value >> 32) & 0xFF);
			b[4] = (uint8)((value >> 24) & 0xFF);
			b[5] = (uint8)((value >> 16) & 0xFF);
			b[6] = (uint8)((value >> 8) & 0xFF);
			b[7] = (uint8)(value & 0xFF);

			this->WriteBytes(&b, sizeof(b));
		}

		inline void WriteUInt64LE(uint64 value)
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
/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *#include "headers/Containers/Strings/UTF-16/CUTF16String.h"
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
#include "UTF-8/UTF8String.hpp"
#include "UTF-16/UTF16String.hpp"
#include "StringUtil.h"

namespace ACStdLib
{
	/**
	 * UTF-16 string that, if needed, transforms itself to UTF-32 so that the string is always fixed length
	 */
    class ACSTDLIB_API String
    {
    private:
        //Members
        bool isUTF32;
        byte storage[sizeof(UTF16String)];
        union
        {
            UTF16String *pStr16;
            CUTF32String *pStr32;
        };

    public:
        //Constructors
        inline String()
        {
            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) UTF16String;
        }

		/**
		 * We assume that c is UTF-8 encoded.
		 * @param c
		 */
        inline String(char c)
        {
            uint16 str[] = {(uint16) c, 0};

            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) UTF16String(str);
        }

		/**
		 * We assume that \p string is UTF-8 encoded.
		 * @param string
		 */
        inline String(const char *string)
        {
            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) UTF16String(UTF8String(string));
        }

        inline String(const UTF8String &refString)
        {
            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) UTF16String(refString);
        }

        inline String(const UTF16String &refString)
        {
            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) UTF16String(refString);
        }

        inline String(const String &refString) //copy ctor
        {
            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) UTF16String;

            *this = refString;
        }

        inline String(String &&refString) //move ctor
        {
            this->pStr16 = nullptr;

            *this = (String &&)refString; //forward
        }

        //Destructor
        inline ~String()
        {
            if(this->pStr16)
                this->pStr16->~UTF16String();
        }

        //Inline operators
		/**
		 * We assume that \p string is UTF-8 encoded.
		 *
		 * @param string
		 * @return
		 */
        inline String &operator=(const char *string)
        {
            *this->pStr16 = UTF8String(string);

            return *this;
        }

        inline String &operator=(const String &refString) //copy assign
        {
            *this->pStr16 = *refString.pStr16;

            return *this;
        }

        inline String &operator=(String &&refString) //move assign
        {
            if(this->pStr16)
                this->pStr16->~UTF16String();

            MemCopy(this->storage, refString.storage, sizeof(this->storage));
            this->pStr16 = (UTF16String *)this->storage;

            refString.pStr16 = nullptr;

            return *this;
        }

        inline String operator+(const String &refRight) const
        {
            return String(*this->pStr16 + *refRight.pStr16);
        }

        inline String &operator+=(char c)
        {
            *this->pStr16 += c;

            return *this;
        }

        inline String &operator+=(uint16 c)
        {
            *this->pStr16 += c;

            return *this;
        }

        inline String &operator+=(uint32 c)
        {
            *this->pStr16 += c;

            return *this;
        }

        inline String &operator+=(const String &refRight)
        {
            *this->pStr16 += *refRight.pStr16;

            return *this;
        }

        inline uint32 operator[](uint32 index) const
        {
            return this->pStr16->GetC_Str()[index];
        }

        inline bool operator==(const String &refOther) const
        {
            return *this->pStr16 == *refOther.pStr16;
        }

        inline bool operator<(const String &refRight) const
        {
            return *this->pStr16 < *refRight.pStr16;
        }

        inline bool operator>(const String &refRight) const
        {
            return *this->pStr16 > *refRight.pStr16;
        }

        //Inline
        inline bool Contains(uint32 codePoint) const
        {
            return this->pStr16->Contains(codePoint);
        }

        inline void EnsureCapacity(uint32 length)
        {
            this->pStr16->EnsureCapacity(length);
        }

        inline uint32 Find(uint16 c, uint32 startPos = 0) const
        {
            return this->pStr16->Find(c, startPos);
        }

        inline int32 FindReverse(uint16 c, uint32 startPos = Natural<uint32>::Max()) const
        {
            return this->pStr16->FindReverse(c, startPos);
        }

        inline uint32 GetLength() const
        {
            return this->pStr16->GetLength();
        }

        inline const UTF16String &GetUTF16() const
        {
            return *this->pStr16;
        }

        inline bool IsEmpty() const
        {
            return this->pStr16->IsEmpty();
        }

        inline String SubString(uint32 beginOffset, uint32 length) const
        {
            return UTF16String(this->pStr16->GetC_Str() + beginOffset, length);
        }

        inline String ToLowercase() const
        {
            return this->pStr16->ToLowercase();
        }
    };

    inline String operator+(char left, const String &refRight)
    {
        return String(left) + refRight;
    }

    inline String operator+(const char *pLeft, const String &refRight)
    {
        return String(pLeft) + refRight;
    }
}
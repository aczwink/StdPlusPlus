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
#include "UTF-16/CUTF16String.h"
#include "String.h"

namespace ACStdLib
{
    /*
    UTF-16 Char that if needed transforms itself to UTF-32 so that the string is always fixed length
    */
    class ACSTDLIB_API CString
    {
    private:
        //Members
        bool isUTF32;
        byte storage[sizeof(CUTF16String)];
        union
        {
            CUTF16String *pStr16;
            CUTF32String *pStr32;
        };

    public:
        //Constructors
        inline CString()
        {
            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) CUTF16String;
        }

        inline CString(char c)
        {
            char str[] = {c, 0};

            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) CUTF16String(str);
        }

        inline CString(const char *pString)
        {
            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) CUTF16String(pString);
        }

        inline CString(const CUTF8String &refString)
        {
            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) CUTF16String(refString);
        }

        inline CString(const CUTF16String &refString)
        {
            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) CUTF16String(refString);
        }

        inline CString(const CString &refString) //copy ctor
        {
            this->isUTF32 = false;
            this->pStr16 = pnew(this->storage) CUTF16String;

            *this = refString;
        }

        inline CString(CString &&refString) //move ctor
        {
            this->pStr16 = nullptr;

            *this = (CString &&)refString; //forward
        }

        //Destructor
        inline ~CString()
        {
            if(this->pStr16)
                this->pStr16->~CUTF16String();
        }

        //Inline operators
        inline CString &operator=(const char *pString)
        {
            *this->pStr16 = pString;

            return *this;
        }

        inline CString &operator=(const CString &refString) //copy assign
        {
            *this->pStr16 = *refString.pStr16;

            return *this;
        }

        inline CString &operator=(CString &&refString) //move assign
        {
            if(this->pStr16)
                this->pStr16->~CUTF16String();

            MemCopy(this->storage, refString.storage, sizeof(this->storage));
            this->pStr16 = (CUTF16String *)this->storage;

            refString.pStr16 = nullptr;

            return *this;
        }

        inline CString operator+(const CString &refRight) const
        {
            return CString(*this->pStr16 + *refRight.pStr16);
        }

        inline CString &operator+=(char c)
        {
            *this->pStr16 += c;

            return *this;
        }

        inline CString &operator+=(uint16 c)
        {
            *this->pStr16 += c;

            return *this;
        }

        inline CString &operator+=(uint32 c)
        {
            *this->pStr16 += c;

            return *this;
        }

        inline CString &operator+=(const CString &refRight)
        {
            *this->pStr16 += *refRight.pStr16;

            return *this;
        }

        inline uint32 operator[](uint32 index) const
        {
            return this->pStr16->GetC_Str()[index];
        }

        inline bool operator==(const CString &refOther) const
        {
            return *this->pStr16 == *refOther.pStr16;
        }

        inline bool operator<(const CString &refRight) const
        {
            return *this->pStr16 < *refRight.pStr16;
        }

        inline bool operator>(const CString &refRight) const
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

        inline int32 FindReverse(uint16 c, uint32 startPos = UINT32_MAX) const
        {
            return this->pStr16->FindReverse(c, startPos);
        }

        inline uint32 GetLength() const
        {
            return this->pStr16->GetLength();
        }

        inline const CUTF16String &GetUTF16() const
        {
            return *this->pStr16;
        }

        inline bool IsEmpty() const
        {
            return this->pStr16->IsEmpty();
        }

        inline CString SubString(uint32 beginOffset, uint32 length) const
        {
            return CUTF16String(this->pStr16->GetC_Str() + beginOffset, length);
        }

        inline CString ToLowercase() const
        {
            return this->pStr16->ToLowercase();
        }
    };

    inline CString operator+(char left, const CString &refRight)
    {
        return CString(left) + refRight;
    }

    inline CString operator+(const char *pLeft, const CString &refRight)
    {
        return CString(pLeft) + refRight;
    }
}
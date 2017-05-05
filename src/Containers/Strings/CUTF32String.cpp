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
//Class Header
#include <ACStdLib/Containers/Strings/CUTF32String.h>
//Local
#include <ACStdLib/Containers/Strings/StringUtil.h>
#include <ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp>
//Namespaces
using namespace ACStdLib;

//Operators
CUTF32String &CUTF32String::operator=(const CUTF32String &refString)
{
	this->EnsureCapacity(refString.nElements);

	this->nElements = refString.nElements;

	MemCopy(this->pData, refString.pData, this->GetSize());
	this->pData[this->nElements] = 0;

	return *this;
}

CUTF32String &CUTF32String::operator=(CUTF32String &&refString)
{
	this->Release();

	this->nElements = refString.nElements;
	this->pData = refString.pData;
	this->capacity = refString.capacity;
	this->elementsAllocInterval = refString.elementsAllocInterval;

	refString.pData = nullptr;

	return *this;
}

CUTF32String &CUTF32String::operator=(uint8 c)
{
	this->EnsureCapacity(1);

	this->pData[0] = c;
	this->pData[1] = 0;

	this->nElements = 1;

	return *this;
}

CUTF32String &CUTF32String::operator=(const uint8 *pString)
{
	uint32 length;

	length = GetStringLength((const char *)pString);
	this->EnsureCapacity(length);

	for(uint32 i = 0; i < length; i++)
		this->pData[i] = pString[i];

	this->nElements = length;
	this->pData[this->nElements] = 0;

	return *this;
}

CUTF32String &CUTF32String::operator=(const uint16 *pString)
{
	uint32 length;

	this->nElements = 0;
	length = GetStringLength(pString);

	this->EnsureCapacity(length);

	for(uint32 i = 0; i < length; i++)
	{
		this->pData[this->nElements] = pString[i];

		this->nElements++;
	}
	this->pData[this->nElements] = 0;

	return *this;
}

CUTF32String &CUTF32String::operator=(const C8BitString &refString)
{
	this->EnsureCapacity(refString.GetLength());

	this->nElements = refString.GetNumberOfElements();
	for(uint32 i = 0; i < this->nElements; i++)
	{
		this->pData[i] = refString[i];
	}
	this->pData[this->nElements] = 0;

	return *this;
}

CUTF32String &CUTF32String::operator = (const UTF8String &refString)
{
	this->EnsureCapacity(refString.GetLength());
	this->nElements = 0;
	for(uint32 codePoint : refString)
	{
		this->pData[this->nElements++] = codePoint;
	}
	this->pData[this->nElements] = 0;

	return *this;
}

CUTF32String &CUTF32String::operator+=(const CUTF32String &refString)
{
	this->EnsureAdditionalCapacity(refString.GetLength());

	MemCopy(&this->pData[this->nElements], refString.pData, refString.GetSize());
	this->nElements += refString.GetLength();
	this->pData[this->nElements] = 0;

	return *this;
}

//Public methods
CUTF32String CUTF32String::SubString(uint32 beginOffset, uint32 length) const
{
	CUTF32String buffer;

	if(length > 0)
	{
		buffer.EnsureCapacity(length);

		MemCopy(buffer.pData, this->pData + beginOffset, length * sizeof(*buffer.pData));
		buffer.nElements = length;
		buffer[length] = 0;
	}

	return buffer;
}

CUTF32String CUTF32String::ToLowercase() const
{
	CUTF32String buffer;

	buffer.EnsureCapacity(this->GetLength());
	for(uint32 i = 0; i < this->GetLength(); i++)
	{
		buffer.pData[i] = ACStdLib::ToLowercase(this->pData[i]);
	}
	buffer.nElements = this->nElements;

	return buffer;
}

//Non-Class Operators
CUTF32String ACStdLib::operator+(const CUTF32String &refLeft, const CUTF32String &refRight)
{
	CUTF32String tmp;

	tmp.EnsureCapacity(refLeft.GetLength() + refRight.GetLength());
	tmp += refLeft;
	tmp += refRight;

	return tmp;
}

bool ACStdLib::operator<(const CUTF32String &refLeft, const CUTF32String &refRight)
{
	int32 cmp;

	if(refLeft.GetLength() < refRight.GetLength())
	{
		cmp = MemCmp(refLeft.GetC_Str(), refRight.GetC_Str(), refLeft.GetSize());

		return cmp ? cmp < 0 : true;
	}
	else if(refLeft.GetLength() > refRight.GetLength())
	{
		cmp = MemCmp(refLeft.GetC_Str(), refRight.GetC_Str(), refRight.GetSize());

		return cmp ? cmp < 0 : false;
	}

	return MemCmp(refLeft.GetC_Str(), refRight.GetC_Str(), refLeft.GetSize()) < 0;
}

bool ACStdLib::operator>(const CUTF32String &refLeft, const CUTF32String &refRight)
{
	int32 cmp;

	if(refLeft.GetLength() < refRight.GetLength())
	{
		cmp = MemCmp(refLeft.GetC_Str(), refRight.GetC_Str(), refLeft.GetSize());

		return cmp ? cmp > 0 : false;
	}
	else if(refLeft.GetLength() > refRight.GetLength())
	{
		cmp = MemCmp(refLeft.GetC_Str(), refRight.GetC_Str(), refRight.GetSize());

		return cmp ? cmp > 0 : true;
	}

	return MemCmp(refLeft.GetC_Str(), refRight.GetC_Str(), refLeft.GetSize()) > 0;
}
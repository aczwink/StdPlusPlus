/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/Streams/StringInputStream.hpp>
//Namespaces
using namespace StdXX;

//Constructor
StringInputStream::StringInputStream(const String &string, bool utf8) : string(string)
{
	if(utf8)
		this->string.ToUTF8();
	else
		this->string.ToUTF16();
	this->bufferInputStream = new BufferInputStream(this->string.GetRawData(), this->string.GetSize());
}

uint32 StringInputStream::GetBytesAvailable() const
{
	return this->bufferInputStream->GetBytesAvailable();
}

uint64 StringInputStream::GetCurrentOffset() const
{
	return this->bufferInputStream->GetCurrentOffset();
}

uint64 StringInputStream::GetRemainingBytes() const
{
	return this->bufferInputStream->GetRemainingBytes();
}

uint64 StringInputStream::GetSize() const
{
	return this->bufferInputStream->GetSize();
}

bool StringInputStream::IsAtEnd() const
{
	return this->bufferInputStream->IsAtEnd();
}

uint32 StringInputStream::ReadBytes(void *destination, uint32 count)
{
	return this->bufferInputStream->ReadBytes(destination, count);
}

void StringInputStream::SetCurrentOffset(uint64 offset)
{
	this->bufferInputStream->SetCurrentOffset(offset);
}

uint32 StringInputStream::Skip(uint32 nBytes)
{
	return this->bufferInputStream->Skip(nBytes);
}
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
#include <ACStdLib/Streams/Readers/CTextReader.h>
//Local
#include <ACStdLib/Debug.h>
#include <ACStdLib/Memory.h>
//Namespaces
using namespace ACStdLib;

//Public methods
C8BitString CTextReader::ReadASCII(uint32 length)
{
    char c;
    C8BitString buffer;

    while(length--)
    {
        c = this->refInput.ReadByte();
        if(this->refInput.HitEnd())
            break;
        buffer += c;
    }

    return buffer;
}

C8BitString CTextReader::ReadASCII_Line()
{
    char c;
    C8BitString buffer;

    while(true)
    {
        c = this->refInput.ReadByte();
        if(c == '\n' || this->refInput.HitEnd())
            break;
        if(c == '\r')
            continue;
        buffer += c;
    }

    return buffer;
}

C8BitString CTextReader::ReadASCII_ZeroTerminated()
{
    char c;
    C8BitString buffer;

    while(true)
    {
        c = this->refInput.ReadByte();
        if(c == 0 || this->refInput.HitEnd())
            break;
        buffer += c;
    }

    return buffer;
}

uint32 CTextReader::ReadUTF8()
{
    byte b;
    uint32 codePoint;

    b = this->refInput.ReadByte();

    if(b & 0x80)
    {
        //more than one byte
        ASSERT(b & 0x40); //this is always set for more than one byte

        if(b & 0x20)
        {
            if(b & 0x10)
            {
                //4 byte
                codePoint = (b & 0xF) << 18;
                codePoint |= (this->refInput.ReadByte() & 0x3F) << 12;
                codePoint |= (this->refInput.ReadByte() & 0x3F) << 6;
                codePoint |= this->refInput.ReadByte() & 0x3F;
            }
            else
            {
                //3 byte
                codePoint = (b & 0xF) << 12;
                codePoint |= (this->refInput.ReadByte() & 0x3F) << 6;
                codePoint |= this->refInput.ReadByte() & 0x3F;
            }
        }
        else
        {
            //2 bytes
            codePoint = (b & 0x1F) << 6;
            codePoint |= this->refInput.ReadByte() & 0x3F;
        }
    }
    else
    {
        //ascii
        codePoint = b;
    }

    return codePoint;
}

UTF8String CTextReader::ReadUTF8Line()
{
    uint32 c;
    UTF8String result;

    while(true)
    {
        c = this->ReadUTF8();
        if(this->refInput.HitEnd() || c == '\n')
            break;
        if(c == '\r')
            continue;

        result += c;
    }

    return result;
}
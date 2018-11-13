/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include "../Streams/InputStream.hpp"
#include "../Streams/OutputStream.hpp"
#include "ResizeableSequenceContainer.hpp"

namespace StdXX
{
    class STDPLUSPLUS_API FIFOBuffer : public ResizeableSequenceContainer<byte>, public InputStream, public OutputStream
    {
    public:
        //Constructors
        FIFOBuffer();

		inline FIFOBuffer(const FIFOBuffer &other)
		{
			*this = other;
		}

		inline FIFOBuffer(FIFOBuffer &&other)
		{
			*this = Move(other);
		}

		//Operators
		FIFOBuffer &operator=(const FIFOBuffer &rhs);
		FIFOBuffer &operator=(FIFOBuffer &&rhs);

        //Methods
        void EnsureCapacity(uint32 requiredNumberOfElements) override;
        bool IsAtEnd() const override;
        uint32 PeekBytes(void *destination, uint32 offset, uint32 count) const;
        uint32 ReadBytes(void *pDestination, uint32 count) override;
        void Release() override;
        uint32 Skip(uint32 nBytes) override;
        uint32 WriteBytes(const void *pSource, uint32 count) override;

        //Inline
        inline uint32 GetRemainingBytes() const
        {
            return (uint32)(this->pCurrentTail - this->pCurrentFront);
        }

    private:
        //Members
        byte *pCurrentFront;
        byte *pCurrentTail;

        //Methods
        void EnsureStorage(uint32 requiredAdditionalSize);
    };
}
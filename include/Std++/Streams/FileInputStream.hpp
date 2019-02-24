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
#include "../Filesystem/Path.hpp"
#include "SeekableInputStream.hpp"

namespace StdXX
{
    class STDPLUSPLUS_API FileInputStream : public SeekableInputStream
    {
    private:
        //Members
        bool hitEnd;
        union
        {
            void *pFileHandle;
            int fileHandle;
        };
    public:
        //Constructor
        FileInputStream(const Path &path);

        //Destructor
        ~FileInputStream();

        //Methods
		uint32 GetBytesAvailable() const override;
        uint64 GetCurrentOffset() const override;
        uint64 GetRemainingBytes() const override;
        uint64 GetSize() const override;
        bool IsAtEnd() const override;
        uint32 ReadBytes(void *pDestination, uint32 count) override;
        void SetCurrentOffset(uint64 offset) override;
        uint32 Skip(uint32 nBytes) override;
	};
}
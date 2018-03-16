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
#include "../Filesystem/Path.hpp"
#include "ASeekableOutputStream.h"

namespace StdPlusPlus
{
    class STDPLUSPLUS_API FileOutputStream : public ASeekableOutputStream
    {
    public:
        //Constructor
        FileOutputStream(const Path &refPath);
        //FileOutputStream
        ~FileOutputStream();
        //Methods
        uint64 GetCurrentOffset() const;
        void SetCurrentOffset(uint64 offset);
        void WriteByte(byte b);
        uint32 WriteBytes(const void *pSource, uint32 size);

		//Inline
		inline const Path &GetPath() const
		{
			return this->filePath;
		}

    private:
        //Members
        union
        {
            void *pFileHandle;
            int fileHandle;
        };
		Path filePath;
    };
}
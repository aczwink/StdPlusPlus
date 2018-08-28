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
#include "../Definitions.h"
#include "Index.hpp"

namespace StdXX
{
    namespace Multimedia
    {
        class BlockIndexEntry
        {
        public:
            //Members
            uint64 offset;
            uint64 size; //when used as a block index. Else this should be set to 0
            uint64 timeStamp;

            //Inline operators
            inline bool operator<(const BlockIndexEntry &refOther) const
            {
                return this->offset < refOther.offset;
            }

			inline bool operator<=(const BlockIndexEntry &refOther) const
			{
				return this->offset <= refOther.offset;
			}

            //Inline
            inline bool Contains(uint64 offset) const
            {
                return (offset >= this->offset) && (offset < this->offset + this->size);
            }

            inline uint64 GetEndOffset()
            {
                return this->offset + this->size;
            }

            inline uint64 GetRemainingBytes(uint64 offset) const
            {
                return this->size - (offset - this->offset);
            }
        };

        class STDPLUSPLUS_API BlockIndex : public Index<BlockIndexEntry>
        {
        public:
            //Methods
            bool FindEntry(uint64 offset, uint32 &entryIndex);
            uint64 GetStartOffset();

            //Inline
			inline void AddEntry(uint64 offset, uint64 size, uint64 timeStamp)
			{
				BlockIndexEntry entry;

				entry.offset = offset;
				entry.size = size;
				entry.timeStamp = timeStamp;

				Index<BlockIndexEntry>::AddEntry(entry);
			}
        };
    }
}
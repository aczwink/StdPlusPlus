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
#include "ACStdLib/Containers/Array/DynamicArray.hpp"
#include "../Definitions.h"

namespace ACStdLib
{
    namespace Multimedia
    {
        class CClusterEntry
        {
        public:
            //Members
            uint64 offset;
            uint64 size;
            uint64 timeStamp;

            //Inline operators
            inline bool operator<(const CClusterEntry &refOther) const
            {
                return this->offset < refOther.offset;
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

        class ACSTDLIB_API ClusterIndex
        {
        private:
            //Members
            bool clustersSorted;
			DynamicArray<CClusterEntry> clusters;

            //Inline
            inline void EnsureSorted()
            {
                if(!this->clustersSorted)
                {
                    this->clusters.Sort();
                    this->clustersSorted = true;
                }
            }

        public:
            //Constructor
            ClusterIndex();

            //Methods
            void AddCluster(uint64 offset, uint64 size, uint64 timeStamp);
            bool FindEntry(uint64 offset, uint32 &refClusterIndex);
            uint64 GetStartOffset();

            //Inline
            inline const CClusterEntry &GetCluster(uint32 index) const
            {
                return this->clusters[index];
            }

            inline uint32 GetNumberOfClusters() const
            {
                return this->clusters.GetNumberOfElements();
            }
        };
    }
}
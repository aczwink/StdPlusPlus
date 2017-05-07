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
#include "../Containers/DynamicArray/DynamicArray.hpp"
#include "AStream.h"
#include "SMetaInfo.h"
#include "SPacket.h"

namespace ACStdLib
{
    namespace Multimedia
    {
        //Forward declarations
        class IFormat;

        class ACSTDLIB_API AMediaGeneric
        {
        protected:
            //Members
            const IFormat &refFormat;
            DynamicArray<AStream *> streams;
            CFraction timeScale;
            uint64 startTime;
            uint64 duration;

            //Methods
            void UpdateTimingInfo();

        public:
            //Members
            SMetaInfo metaInfo;

            //Constructor
            inline AMediaGeneric(const IFormat &refFormat) : refFormat(refFormat)
            {
                //Time base for all timestamps in this context (NOT for the stream timestamps)
                //this->timeScale = CFraction(1, 1000000); //the smaller the value, the better... it's more precise like that

                this->startTime = UINT64_MAX;
                this->duration = UINT64_MAX;

                this->metaInfo.trackNumber = 0;
                this->metaInfo.year = 0;
            }

            //Destructor
            virtual ~AMediaGeneric();

            //Inline
            inline uint32 AddStream(AStream *pStream)
            {
                return this->streams.Push(pStream);
            }

            inline uint64 GetDuration() const
            {
                return this->duration;
            }

            inline uint32 GetNumberOfStreams() const
            {
                return this->streams.GetNumberOfElements();
            }

            inline AStream *GetStream(uint32 index)
            {
                return this->streams[index];
            }

            inline const AStream *GetStream(uint32 index) const
            {
                return this->streams[index];
            }

            inline const CFraction &GetTimeScale() const
            {
                return this->timeScale;
            }
        };
    }
}
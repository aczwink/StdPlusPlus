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
#include "Stream.hpp"
#include "SMetaInfo.h"
#include "Packet.hpp"

namespace ACStdLib
{
    namespace Multimedia
    {
        //Forward declarations
        class Format;

        class ACSTDLIB_API MediaObject
        {
        protected:
            //Members
            const Format &refFormat;
            DynamicArray<Stream *> streams;
            Fraction timeScale;
            uint64 startTime;
            uint64 duration;

            //Methods
            void UpdateTimingInfo();

        public:
            //Members
            SMetaInfo metaInfo;

            //Constructor
            inline MediaObject(const Format &refFormat) : refFormat(refFormat)
            {
                //Time base for all timestamps in this context (NOT for the stream timestamps)
                //this->timeScale = Fraction(1, 1000000); //the smaller the value, the better... it's more precise like that

                this->startTime = Natural<uint64>::Max();
                this->duration = Natural<uint64>::Max();

                this->metaInfo.trackNumber = 0;
                this->metaInfo.year = 0;
            }

            //Destructor
            virtual ~MediaObject();

            //Inline
            inline uint32 AddStream(Stream *pStream)
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

            inline Stream *GetStream(uint32 index)
            {
                return this->streams[index];
            }

            inline const Stream *GetStream(uint32 index) const
            {
                return this->streams[index];
            }

            inline const Fraction &GetTimeScale() const
            {
                return this->timeScale;
            }
        };
    }
}
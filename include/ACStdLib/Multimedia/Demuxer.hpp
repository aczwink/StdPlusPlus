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
#include "ACStdLib/Streams/SeekableInputStream.hpp"
#include "MediaObject.hpp"
#include "Packet.hpp"

namespace ACStdLib
{
    namespace Multimedia
    {
        class ACSTDLIB_API Demuxer : public MediaObject
        {
        private:
            //Members
            uint32 bitRate;

            //Methods
            bool AllInfoIsAvailable();
            bool AllStreamsHaveDuration() const;
            void DeriveDurationFromPacketTimestamps();
            void ExtractInfo(Packet &refPacket);

        protected:
            //Members
            SeekableInputStream &inputStream;

        public:
            //Constructor
            inline Demuxer(const Format &refFormat, SeekableInputStream &refInput) : MediaObject(refFormat), inputStream(refInput)
            {
                this->bitRate = 0;
            }

            //Abstract
            virtual void ReadHeader() = NULL;
            virtual bool ReadPacket(Packet &refPacket) = NULL;

            //Methods
            bool FindStreamInfo();
            bool ReadFrame(Packet &packet);

            //Inline
            inline uint32 GetBitRate() const
            {
                return this->bitRate;
            }

            inline uint64 GetStartTime() const
            {
                return this->startTime;
            }
        };
    }
}
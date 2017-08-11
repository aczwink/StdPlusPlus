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
#include "../Containers/LinkedList/LinkedList.hpp"
#include "CodecId.hpp"
#include "Packet.hpp"

namespace ACStdLib
{
    namespace Multimedia
    {
        class ACSTDLIB_API AParser
        {
        private:
            //Members
            LinkedList<Packet> parsedFrames;

        protected:
            //Members
            Packet frameBuffer;

            //Methods
            void AddToFrameBuffer(const Packet &refPacket);
            void AddToFrameBuffer(const void *pData, uint32 size);
            void ReadyFrameBuffer(uint32 nOverreadBytes = 0);

            //Inline
            inline void AddFrameHeader(uint32 header)
            {
                byte b[4];

                b[0] = byte(header >> 24);
                b[1] = byte(header >> 16 & 0xFF);
                b[2] = byte(header >> 8 & 0xFF);
                b[3] = byte(header & 0xFF);

                this->AddToFrameBuffer(b, 4);
            }

        public:
            //Constructor
            AParser();

            //Destructor
            virtual ~AParser();

            //Abstract
            virtual CodecId GetCodecId() const = 0;
            virtual void Parse(const Packet &refPacket) = 0;

            //Overrideable
            virtual void Reset();

            //Inline
            inline void GetParsedFrame(Packet &refPacket)
            {
                ASSERT(this->IsFrameReady());

                refPacket = parsedFrames.PopFront();
            }

            inline bool IsFrameReady() const
            {
                return !this->parsedFrames.IsEmpty();
            }
        };
    }
}
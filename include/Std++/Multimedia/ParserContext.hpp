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
#include "../Containers/LinkedList/LinkedList.hpp"
#include "Packet.hpp"

namespace StdXX
{
    namespace Multimedia
    {
		//Forward declarations
		class Stream;

        class STDPLUSPLUS_API ParserContext
        {
		public:
			//Constructor
			ParserContext(Stream& stream);
			
			//Destructor
			virtual ~ParserContext();

			//Abstract
			//virtual CodecId GetCodecId() const = 0;
			virtual void Parse(const Packet &refPacket) = 0;

			//Overrideable
			virtual void Reset();

			//Inline
			inline void GetParsedFrame(Packet &refPacket)
			{
				ASSERT(this->IsFrameReady(), "If you see this, report to Std++");

				refPacket = parsedFrames.PopFront();
			}

			inline bool IsFrameReady() const
			{
				return !this->parsedFrames.IsEmpty();
			}

			inline bool ShouldRepack() const
			{
				return this->repack;
			}

        protected:
            //Members
			Stream& stream;
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

		private:
			//Members
			bool repack;
			LinkedList<Packet> parsedFrames;
        };
    }
}
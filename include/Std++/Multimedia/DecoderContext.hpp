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
#include "../Containers/PriorityQueue.hpp"
#include "Frame.hpp"
#include "Packet.hpp"

namespace StdXX
{
	namespace Multimedia
	{
		//Forward declarations
		class Decoder;
		class Stream;

		class STDPLUSPLUS_API DecoderContext
		{
		public:
			//Constructor
			inline DecoderContext(const Decoder &decoder, Stream &stream) : decoder(decoder), stream(stream)
			{
				this->frameCounter = 0;
			}

			//Destructor
			virtual ~DecoderContext();

			//Abstract
			virtual void Decode(const Packet &packet) = 0;

			//Overrideable
			virtual void Reset();

			//Inline
			inline const Decoder &GetDecoder() const
			{
				return this->decoder;
			}

			inline Frame *GetNextFrame()
			{
				return this->orderedFrames.PopFront();
			}

			inline bool IsFrameReady() const
			{
				return !this->orderedFrames.IsEmpty();
			}

		protected:
			//Members
			Stream &stream;

			//Methods
			void AddFrame(Frame *pFrame, uint32 frameNumber = 0);

			//Inline
			inline void ResetFrameCounter()
			{
				this->frameCounter = 0;
			}

		private:
			//Members
			const Decoder &decoder;
			uint32 frameCounter;
			PriorityQueue<Frame *> unorderedFrames;
			LinkedList<Frame *> orderedFrames;
		};
	}
}
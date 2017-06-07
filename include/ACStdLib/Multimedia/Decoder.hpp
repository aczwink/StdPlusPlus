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
#include "ACStdLib/Containers/PriorityQueue.hpp"
#include "Frame.hpp"
#include "Packet.hpp"

namespace ACStdLib
{
    namespace Multimedia
    {
        class ACSTDLIB_API Decoder
        {
        public:
            //Constructor
            inline Decoder()
            {
                this->frameCounter = 0;
            }

            //Destructor
            virtual ~Decoder();

            //Abstract
            virtual void Decode(const Packet &packet) = NULL;

            //Overrideable
            virtual void Reset();

            //Inline
            inline Frame *GetNextFrame()
            {
                return this->orderedFrames.PopFront();
            }

            inline bool IsFrameReady() const
            {
                return !this->orderedFrames.IsEmpty();
            }

        private:
            //Members
            uint32 frameCounter;
            PriorityQueue<Frame *> unorderedFrames;
            LinkedList<Frame *> orderedFrames;

		protected:
			//Methods
			void AddFrame(Frame *pFrame, uint32 frameNumber = 0);

			//Inline
			inline void ResetFrameCounter()
			{
				this->frameCounter = 0;
			}
        };
    }
}
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
#include "AFrame.h"
#include "SPacket.h"

namespace ACStdLib
{
    namespace Multimedia
    {
        class ACSTDLIB_API ADecoder
        {
        private:
            //Members
            uint32 frameCounter;
            PriorityQueue<AFrame *> unorderedFrames;
            LinkedList<AFrame *> orderedFrames;

        protected:
            //Methods
            void AddFrame(AFrame *pFrame, uint32 frameNumber = 0);

            //Inline
            inline void ResetFrameCounter()
            {
                this->frameCounter = 0;
            }

        public:
            //Constructor
            inline ADecoder()
            {
                this->frameCounter = 0;
            }

            //Destructor
            virtual ~ADecoder();

            //Abstract
            virtual void Decode(const SPacket &refPacket) = NULL;

            //Overrideable
            virtual void Reset();

            //Inline
            inline AFrame *GetNextFrame()
            {
                return this->orderedFrames.PopFront();
            }

            inline bool IsFrameReady() const
            {
                return !this->orderedFrames.IsEmpty();
            }
        };
    }
}
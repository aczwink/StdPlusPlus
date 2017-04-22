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
#include "../Definitions.h"
#include "AAudioBuffer.h"
#include "../CFunction.h"

namespace ACStdLib
{
    namespace Multimedia
    {
        struct SAudioPlaybackFormat
        {
            EAudioSampleType sampleType;
            uint32 sampleRate;
            uint8 nChannels;
        };

        class ACSTDLIB_API CAudioVoice
        {
        private:
            //Members
            void *pOSHandle;

        public:
            //Constructor
            CAudioVoice(const SAudioPlaybackFormat &refPlaybackFormat);

            //Destructor
            ~CAudioVoice();

            //Methods
            void BindBufferEndEvent(const CFunction<void (const void *)> &refCallback);
            uint32 GetMaximumNumberOfQueuedBuffers();
            void Start();
            void Stop();
            void SubmitBuffer(const void *pBuffer, uint32 size);
        };
    }
}
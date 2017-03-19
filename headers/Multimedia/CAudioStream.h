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
#include "AStream.h"
#include "Definitions.h"

namespace ACStdLib
{
    namespace Multimedia
    {
        class ACSTDLIB_API CAudioStream : public AStream
        {
        private:
            //Methods
            bool AllDecoderInfoIsAvailable();
        public:
            //Members
            uint8 nChannels;
            uint32 sampleRate;

            //Constructor
            inline CAudioStream()
            {
                this->nChannels = 0;
                this->sampleRate = 0;
            }

            //Methods
            EDataType GetType() const;

            //Inline
            inline EChannelLayout GetChannelLayout() const
            {
                switch(this->nChannels)
                {
                    case 1:
                        return EChannelLayout::Mono;
                    case 2:
                        return EChannelLayout::Stereo;
                }

                NOT_IMPLEMENTED_ERROR;
            }
        };
    }
}
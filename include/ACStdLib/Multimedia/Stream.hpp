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
#include "ACStdLib/Math/Fraction.hpp"
#include "AParser.h"
#include "EnumTypes.hpp"
#include "Codec.hpp"
#include "Decoder.hpp"
#include "Encoder.hpp"

namespace ACStdLib
{
    namespace Multimedia
    {
        class ACSTDLIB_API Stream
        {
        private:
            //Members
            const Codec *pCodec;
            Decoder *pDecoder;
            Encoder *pEncoder;
            AParser *pParser;

            //Abstract
            virtual bool AllDecoderInfoIsAvailable() = 0;

        public:
            //Members
            Fraction timeScale;
            uint64 startTime;
            uint64 duration;
            bool vbr;
            uint32 bitRate; //if vbr then this is the average bitrate

            //Constructor
            Stream();

            //Destructor
            ~Stream();

            //Abstract
            virtual DataType GetType() const = 0;

            //Methods
            bool AllInfoIsAvailable();
            Decoder *GetDecoder();
            Encoder *GetEncoder();
            AParser *GetParser();

            //Inline
            inline const Codec *GetCodec() const
            {
                return this->pCodec;
            }

            inline void SetCodec(CodecId codecId)
            {
                this->pCodec = Codec::GetCodec(codecId);
            }

            inline void SetCodec(const Codec *pCodec)
            {
                this->pCodec = pCodec;
            }

            inline void SetParser(AParser *pParser)
            {
                this->pParser = pParser;
            }
        };
    }
}
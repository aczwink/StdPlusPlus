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
#include "../Math/CFraction.h"
#include "AParser.h"
#include "EDataType.h"
#include "ICodec.h"
#include "ADecoder.h"
#include "IEncoder.h"

namespace ACStdLib
{
    namespace Multimedia
    {
        class ACSTDLIB_API AStream
        {
        private:
            //Members
            const ICodec *pCodec;
            ADecoder *pDecoder;
            IEncoder *pEncoder;
            AParser *pParser;

            //Abstract
            virtual bool AllDecoderInfoIsAvailable() = NULL;

        public:
            //Members
            CFraction timeScale;
            uint64 startTime;
            uint64 duration;
            bool vbr;
            uint32 bitRate; //if vbr then this is the average bitrate

            //Constructor
            AStream();

            //Destructor
            ~AStream();

            //Abstract
            virtual EDataType GetType() const = NULL;

            //Methods
            bool AllInfoIsAvailable();
            ADecoder *GetDecoder();
            IEncoder *GetEncoder();
            AParser *GetParser();

            //Inline
            inline const ICodec *GetCodec() const
            {
                return this->pCodec;
            }

            inline void SetCodec(ECodecId codecId)
            {
                this->pCodec = ICodec::GetCodec(codecId);
            }

            inline void SetCodec(const ICodec *pCodec)
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
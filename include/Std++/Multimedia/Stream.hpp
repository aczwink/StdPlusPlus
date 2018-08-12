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
#include "../Definitions.h"
#include "../Math/Fraction.hpp"
#include "ParserContext.hpp"
#include "EnumTypes.hpp"
#include "CodingFormat.hpp"
#include "DecoderContext.hpp"
#include "EncoderContext.hpp"

namespace StdXX
{
    namespace Multimedia
    {
        class STDPLUSPLUS_API Stream
        {
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
            virtual ~Stream();

            //Abstract
            virtual DataType GetType() const = 0;

            //Methods
            bool AllInfoIsAvailable();

            //Inline
            inline const CodingFormat *GetCodingFormat() const
            {
                return this->codingFormat;
            }

			inline DecoderContext *GetDecoderContext()
			{
				return this->decoderContext;
			}

			inline EncoderContext *GetEncoderContext()
			{
				return this->encoderContext;
			}

			inline ParserContext *GetParserContext()
			{
				return this->parserContext;
			}

            inline void SetCodingFormat(CodingFormatId codingFormatId)
            {
				this->codingFormat = CodingFormat::GetCodingFormatById(codingFormatId);
            }

			inline void SetDecoderContext(DecoderContext *decoderContext)
			{
				delete this->decoderContext;
				this->decoderContext = decoderContext;
			}

			inline void SetEncoderContext(EncoderContext *encoderContext)
			{
				delete this->encoderContext;
				this->encoderContext = encoderContext;
			}

            inline void SetParserContext(ParserContext *parserContext)
            {
                this->parserContext = parserContext;
            }

		private:
			//Members
			const CodingFormat *codingFormat;
			DecoderContext *decoderContext;
			EncoderContext *encoderContext;
			ParserContext *parserContext;

			//Abstract
			virtual bool AllDecoderInfoIsAvailable() = 0;
        };
    }
}
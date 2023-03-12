/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Optional.hpp>
#include "../Definitions.h"
#include "Std++/Math/Rational.hpp"
#include "ParserContext.hpp"
#include "EnumTypes.hpp"
#include "CodingFormat.hpp"
#include "DecoderContext.hpp"
#include "EncoderContext.hpp"
#include "TimeScale.hpp"
#include "CodingParameters.hpp"
#include "FormatRegistry.hpp"

namespace StdXX::Multimedia
{
	class STDPLUSPLUS_API Stream
	{
	public:
		//Members
		const Decoder* decoder;
		TimeScale timeScale;
		uint64 startTime;
		uint64 duration;
		CodingParameters codingParameters;
		struct
		{
			/**
			 * Sometimes streams are correctly muxed on frame boundaries and don't need parsing.
			 * We can in that case skip this process and thus increase performance
			 */
			bool requiresParsing;
			/**
			 * Whether the parser should repack the packets or not.
			 * If true, parsers will repack packages so that they are friendly for the decoder (packed to frame boundaries), which will cause an overhead on performance.
			 * If false, the parser will look at the packets and extract info from them but do not repack them. This is faster than decoding and might still gives info like timestamps etc..
			 * This is especially useful if no decoder is available.
			 */
			bool repack;
		} parserFlags;

		//Constructor
		Stream(DataType dataType);

		//Destructor
		virtual ~Stream();

		//Methods
		bool AllDecodingInfoIsAvailable();
		bool AllInfoIsAvailable();

		//Inline
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
			this->codingParameters.codingFormat = FormatRegistry::GetCodingFormatById(codingFormatId);
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
		DecoderContext *decoderContext;
		EncoderContext *encoderContext;
		ParserContext *parserContext;
	};
}
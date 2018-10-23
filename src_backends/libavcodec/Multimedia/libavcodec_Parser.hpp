/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
//Global
extern "C"
{
#include <libavcodec/avcodec.h>
}
//Local
#include <Std++/Multimedia/Parser.hpp>
#include "../../../src/Multimedia/CodingFormatIdMap.hpp"

namespace _stdxx_
{
	class libavcodec_Parser : public StdXX::Multimedia::Parser
	{
	public:
		//Constructor
		inline libavcodec_Parser(const CodingFormatIdMap<AVCodecID> &libavCodecIdMap, StdXX::Multimedia::CodingFormatId codingFormatId, AVCodecID libavCodecId) :
			codingFormatId(codingFormatId), libavCodecId(libavCodecId), libavCodecIdMap(libavCodecIdMap)
		{
			AVCodecParserContext *parserContext = av_parser_init(libavCodecId);
			this->parser = parserContext->parser;
			av_parser_close(parserContext);
		}

		//Methods
		StdXX::Multimedia::ParserContext * CreateContext(StdXX::Multimedia::Stream& stream) const override;
		StdXX::FixedArray<StdXX::Multimedia::CodingFormatId> GetCodingFormatIds() const override;

	private:
		//Members
		StdXX::Multimedia::CodingFormatId codingFormatId;
		AVCodecID libavCodecId;
		const CodingFormatIdMap<AVCodecID> &libavCodecIdMap;
		AVCodecParser *parser;
	};
}
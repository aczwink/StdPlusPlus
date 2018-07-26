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
#ifdef _AC_LIB_USEAVCODEC
extern "C"
{
#include <libavcodec/avcodec.h>
}
//Local
#include <StdXX/Containers/DynamicArray/DynamicArray.hpp>
#include <StdXX/Multimedia/CodecId.hpp>
#include <StdXX/Multimedia/Decoder.hpp>
#include <StdXX/Multimedia/Packet.hpp>
#include <StdXX/Multimedia/Stream.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

struct CodecState
{
	AVPacket *pkt;
	const AVCodec *codec;
	AVCodecContext *codecContext;
	AVCodecParserContext *parser;
	AVFrame *frame;
};

//Functions
void DecodePacket(CodecState &state, const StdXX::Multimedia::Packet &packet, StdXX::DynamicArray<StdXX::Multimedia::Frame *> &frames);
void FreeCodecState(CodecState &state);
void InitCodecState(CodecState &state, StdXX::Multimedia::CodecId codecId, Stream &stream);
AVCodecID MapCodecId(StdXX::Multimedia::CodecId codecId);
#endif
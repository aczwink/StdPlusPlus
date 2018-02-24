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
#include <StdPlusPlus/Containers/DynamicArray/DynamicArray.hpp>
#include <StdPlusPlus/Multimedia/CodecId.hpp>
#include <StdPlusPlus/Multimedia/Decoder.hpp>
#include <StdPlusPlus/Multimedia/Packet.hpp>
#include <StdPlusPlus/Multimedia/Stream.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Multimedia;

struct CodecState
{
	AVPacket *pkt;
	const AVCodec *codec;
	AVCodecContext *codecContext;
	AVCodecParserContext *parser;
	AVFrame *frame;
};

//Functions
void DecodePacket(CodecState &state, const StdPlusPlus::Multimedia::Packet &packet, StdPlusPlus::DynamicArray<StdPlusPlus::Multimedia::Frame *> &frames);
void FreeCodecState(CodecState &state);
void InitCodecState(CodecState &state, StdPlusPlus::Multimedia::CodecId codecId, Stream &stream);
AVCodecID MapCodecId(StdPlusPlus::Multimedia::CodecId codecId);
#endif
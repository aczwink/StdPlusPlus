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
extern "C"
{
#include <libavcodec/avcodec.h>
}
//Local
#include <ACStdLib/Containers/DynamicArray/DynamicArray.hpp>
#include <ACStdLib/Multimedia/CodecId.hpp>
#include <ACStdLib/Multimedia/Decoder.hpp>
#include <ACStdLib/Multimedia/Packet.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;

struct CodecState
{
	AVPacket *pkt;
	const AVCodec *codec;
	AVCodecContext *codecContext;
	AVCodecParserContext *parser;
	AVFrame *frame;
};

//Functions
void DecodePacket(CodecState &state, const ACStdLib::Multimedia::Packet &packet, ACStdLib::DynamicArray<ACStdLib::Multimedia::Frame *> &frames);
void InitCodecState(CodecState &state, ACStdLib::Multimedia::CodecId codecId);
AVCodecID MapCodecId(ACStdLib::Multimedia::CodecId codecId);
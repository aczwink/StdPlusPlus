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
#include "LibAVCodec.hpp"
//Local
#include <ACStdLib/Containers/Map/Map.hpp>
#include <ACStdLib/Multimedia/AudioBuffer.hpp>
#include <ACStdLib/Multimedia/AudioFrame.hpp>

//Global variables
ACStdLib::Map<ACStdLib::Multimedia::CodecId, AVCodecID> g_libavcodec_codec_map;

//Local functions
static void LoadMap()
{
	static bool loaded = false;

	if(!loaded)
	{
		loaded = true;

		avcodec_register_all();

		g_libavcodec_codec_map.Insert(ACStdLib::Multimedia::CodecId::MP3, AV_CODEC_ID_MP3);
	}
}

static ChannelLayout MapChannels(int nChannels)
{
	switch(nChannels)
	{
		case 1:
			return ChannelLayout::Mono;
		case 2:
			return ChannelLayout::Stereo;
	}

	NOT_IMPLEMENTED_ERROR;
}

//Functions
void DecodePacket(CodecState &state, const ACStdLib::Multimedia::Packet &packet, ACStdLib::DynamicArray<ACStdLib::Multimedia::Frame *> &frames)
{
	const byte *data = packet.GetData();
	uint32 leftSize = packet.GetSize();

	while(leftSize)
	{
		//parse frames from our packet
		int ret = av_parser_parse2(state.parser, state.codecContext, &state.pkt->data, &state.pkt->size, data, leftSize, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
		if(ret < 0)
			break; //an error occured. skip packet

		data += ret;
		leftSize -= ret;

		if(state.pkt->size)
		{
			ret = avcodec_send_packet(state.codecContext, state.pkt);
			if(ret < 0)
				break; //an error occured. skip packet

			while(ret >= 0)
			{
				ret = avcodec_receive_frame(state.codecContext, state.frame);
				if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
					return;
				else if(ret < 0)
					return; //an error occured. skip packet

				//ready frame
				switch(state.codec->type)
				{
					case AVMEDIA_TYPE_AUDIO:
					{
						switch(state.frame->format)
						{
							case AV_SAMPLE_FMT_S16P:
							{
								AudioBuffer<int16> *buffer = new AudioBuffer<int16>(MapChannels(state.frame->channels), (uint32) state.frame->nb_samples);
								for(uint32 i = 0; i < state.frame->channels; i++)
								{
									MemCopy(buffer->GetChannel((Channel)i), state.frame->data[i], state.frame->nb_samples * sizeof(int16));
								}
								frames.Push(new AudioFrame(buffer));
							}
							break;
							default:
								NOT_IMPLEMENTED_ERROR;
						}
					}
					break;
				}
			}
		}
	}
}

void InitCodecState(CodecState &state, ACStdLib::Multimedia::CodecId codecId)
{
	state.pkt = av_packet_alloc();
	state.codec = avcodec_find_decoder(MapCodecId(codecId));
	state.parser = av_parser_init(state.codec->id);
	state.codecContext = avcodec_alloc_context3(state.codec);
	state.frame = av_frame_alloc();

	int ret = avcodec_open2(state.codecContext, state.codec, nullptr);
}

AVCodecID MapCodecId(ACStdLib::Multimedia::CodecId codecId)
{
	LoadMap();

	auto it = g_libavcodec_codec_map.Find(codecId);
	if(it == g_libavcodec_codec_map.end())
		return AV_CODEC_ID_NONE;

	return (*it).value;
}
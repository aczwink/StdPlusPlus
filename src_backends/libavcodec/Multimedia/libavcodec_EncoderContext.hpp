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
#include <Std++/Multimedia/AudioFrame.hpp>
#include <Std++/Multimedia/EncoderContext.hpp>
#include <Std++/Multimedia/CodingFormatId.hpp>
#include <Std++/Multimedia/Stream.hpp>
#include <Std++/Multimedia/PixelFormat.hpp>
#include <Std++/Multimedia/VideoFrame.hpp>
#include <Std++/Containers/Map/BijectiveMap.hpp>

namespace _stdxx_
{
	class libavcodec_EncoderContext : public StdXX::Multimedia::EncoderContext
	{
	public:
		//Constructor
		libavcodec_EncoderContext(StdXX::Multimedia::Stream &stream, AVCodec *codec,
								  const StdXX::BijectiveMap<StdXX::Multimedia::NamedPixelFormat, AVPixelFormat> &libavPixelFormatMap);

		//Destructor
		~libavcodec_EncoderContext();

		//Methods
		void Encode(const StdXX::Multimedia::Frame &frame) override;
		void Flush() override;

	private:
		//Members
		const StdXX::BijectiveMap<StdXX::Multimedia::NamedPixelFormat, AVPixelFormat> &libavPixelFormatMap;
		AVCodecContext *codecContext;
		AVFrame *frame;
		AVPacket *packet;
		StdXX::Multimedia::NamedPixelFormat namedPixelFormat;

		//Methods
		void Encode(AVFrame *frame);
		void MapAudioFrame(const StdXX::Multimedia::AudioFrame &audioFrame) const;
		void MapPacket();
		void MapVideoFrame(const StdXX::Multimedia::VideoFrame &frame) const;
	};
}
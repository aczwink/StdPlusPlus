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
#include <Std++/Multimedia/Encoder.hpp>
#include <Std++/Multimedia/CodecId.hpp>
#include <Std++/Multimedia/Stream.hpp>

namespace _stdxx_
{
	class libavcodec_Encoder : public StdXX::Multimedia::Encoder
	{
	public:
		//Constructor
		libavcodec_Encoder(StdXX::Multimedia::Stream &stream, StdXX::Multimedia::CodecId codecId);

		//Destructor
		~libavcodec_Encoder();

		//Methods
		void Encode(const StdXX::Multimedia::Frame &frame) override;
		void Flush() override;

	private:
		//Members
		const AVCodec *codec;
		AVCodecContext *codecContext;
		AVFrame *frame;
		AVPacket *packet;

		//Methods
		void Encode(AVFrame *frame);
		void MapFrame(const StdXX::Multimedia::Frame &frame) const;
		void MapPacket();
	};
}
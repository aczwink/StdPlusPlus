/*
* Copyright (c) 2018-2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/AudioSampleFormat.hpp>
#include <Std++/Multimedia/DecoderContext.hpp>
#include <Std++/Multimedia/Stream.hpp>
#include <Std++/Multimedia/PixelFormat.hpp>
#include <Std++/Containers/BinaryTreeMap/BijectiveMap.hpp>

namespace _stdxx_
{
	class libavcodec_DecoderContext : public StdXX::Multimedia::DecoderContext
	{
	public:
		//Constructor
		libavcodec_DecoderContext(const StdXX::Multimedia::Decoder &decoder, StdXX::Multimedia::Stream &stream, const AVCodec *codec);

		//Destructor
		~libavcodec_DecoderContext();

		//Methods
		void Decode(const StdXX::Multimedia::IPacket & packet) override;
		void Reset() override;

	private:
		//Members
		AVCodecContext *codecContext;
		AVFrame *frame;
		byte* extradata;

		//Methods
		uint64 GetBestFramePTS() const;
		void MapAudioFrame();
		void MapVideoFrame();
	};
}
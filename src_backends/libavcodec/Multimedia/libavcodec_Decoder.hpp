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
#include <Std++/Multimedia/Decoder.hpp>
#include <Std++/Multimedia/PixelFormat.hpp>
#include <Std++/Containers/Map/BijectiveMap.hpp>

namespace _stdxx_
{
	class libavcodec_Decoder : public StdXX::Multimedia::Decoder
	{
	public:
		//Constructor
		inline libavcodec_Decoder(const StdXX::BijectiveMap<StdXX::Multimedia::NamedPixelFormat, AVPixelFormat> &libavPixelFormatMap, StdXX::Multimedia::CodingFormatId codingFormatId, AVCodecID libavCodecId)
			: libavPixelFormatMap(libavPixelFormatMap), codingFormatId(codingFormatId)
		{
			this->codec = avcodec_find_decoder(libavCodecId);
		}

		//Methods
		StdXX::Multimedia::DecoderContext * CreateContext(StdXX::Multimedia::Stream & stream) const override;
		StdXX::Multimedia::CodingFormatId GetCodingFormatId() const override;
		StdXX::String GetName() const override;

	private:
		//Members
		const StdXX::BijectiveMap<StdXX::Multimedia::NamedPixelFormat, AVPixelFormat> &libavPixelFormatMap;
		StdXX::Multimedia::CodingFormatId codingFormatId;
		AVCodec *codec;
	};
}
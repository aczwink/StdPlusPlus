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
#ifdef _STDXX_EXTENSION_LIBAVCODEC
//Global
extern "C"
{
#include <libavcodec/avcodec.h>
}
//Local
#include <Std++/_Backends/Extension.hpp>
#include <Std++/Multimedia/CodingFormatId.hpp>
#include <Std++/Multimedia/PixelFormat.hpp>
#include "../../src/Multimedia/CodingFormatIdMap.hpp"

namespace _stdxx_
{
	class libavcodec_Backend : public Extension
	{
	public:
		//Methods
		void Load() override;

	private:
		//Members
		CodingFormatIdMap<AVCodecID> libavCodecIdMap;
		StdXX::BijectiveMap<StdXX::Multimedia::NamedPixelFormat, AVPixelFormat> libavPixelFormatMap;

		//Methods
		void LoadCodingFormatIdMap();
#ifdef _STDXX_EXTENSION_LIBAVCODEC_ENABLE_PATENDED
		void LoadPatentedCodingFormatIdMap();
#endif
		void LoadPixelFormatMap();
		void RegisterDecoderIfAvailable(StdXX::Multimedia::CodingFormatId codingFormatId, AVCodecID libavCodecId);
		void RegisterEncoderIfAvailable(StdXX::Multimedia::CodingFormatId codingFormatId, AVCodecID libavCodecId);
		void RegisterParserIfAvailable(StdXX::Multimedia::CodingFormatId codingFormatId, AVCodecID libavCodecId);
	};
}
#endif
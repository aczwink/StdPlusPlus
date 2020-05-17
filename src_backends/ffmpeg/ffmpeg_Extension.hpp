/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Global
extern "C"
{
#include <libavcodec/avcodec.h>
}
//Local
#include <Std++/_Backends/Extension.hpp>
#include <Std++/Multimedia/AudioSampleFormat.hpp>
#include <Std++/Multimedia/PixelFormat.hpp>
#include "../../src/Multimedia/CodingFormatIdMap.hpp"

namespace _stdxx_
{
	class ffmpeg_Extension : public Extension
	{
	public:
		//Methods
		void Load() override;
		StdXX::Multimedia::AudioSampleFormat MapAudioSampleFormat(int nChannels, AVSampleFormat sampleFormat) const;

		//Inline
		inline StdXX::Multimedia::CodingFormatId MapCodecId(AVCodecID avCodecId) const
		{
			return this->libavCodecIdMap.Get(avCodecId);
		}

		inline StdXX::Multimedia::PixelFormat MapPixelFormat(AVPixelFormat avPixelFormat) const
		{
			StdXX::Multimedia::NamedPixelFormat name = this->libavPixelFormatMap.GetReverse(avPixelFormat);
			return StdXX::Multimedia::PixelFormat(name);
		}

		inline AVPixelFormat MapPixelFormat(StdXX::Multimedia::NamedPixelFormat namedPixelFormat) const
		{
			return this->libavPixelFormatMap.Get(namedPixelFormat);
		}

	protected:
		//Members
		CodingFormatIdMap<AVCodecID> libavCodecIdMap;

		//Methods
		void LoadCodingFormatIdMap();
		void LoadPatentedCodingFormatIdMap();

	private:
		//Members
		StdXX::BijectiveMap<StdXX::Multimedia::NamedPixelFormat, AVPixelFormat> libavPixelFormatMap;

		//Methods
		void LoadPixelFormatMap();
	};
}
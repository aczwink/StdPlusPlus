/*
 * Copyright (c) 2017-2024 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/Containers/Optional.hpp>
#include <Std++/Math/Size.hpp>
#include <Std++/Buffers/FixedSizeBuffer.hpp>
#include <Std++/Math/Rational.hpp>
#include "AudioSampleFormat.hpp"
#include "PixelFormat.hpp"
#include "CodingFormat.hpp"
#include "TimeScale.hpp"

namespace StdXX::Multimedia
{
	struct AudioCodingParameters
	{
		uint32 sampleRate = 0;
		Optional<AudioSampleFormat> sampleFormat;
	};

	struct VideoCodingParameters
	{
		Optional<PixelFormat> pixelFormat;
		Math::Size<uint16> size;

		//Properties
		inline Math::Rational<uint16> AspectRatio() const
		{
			return Math::Rational(this->size.width, this->size.height).Reduce();
		}
	};

	struct DecodingParameters
	{
		DataType dataType;
		const CodingFormat *codingFormat = nullptr;
		Optional<bool> vbr;
		uint32 bitRate = 0; //if vbr then this is the average bitrate
		Optional<FixedSizeBuffer> codecPrivateData;

		AudioCodingParameters audio;
		VideoCodingParameters video;
	};

	struct EncodingParameters : public DecodingParameters
	{
		TimeScale timeScale;

		inline EncodingParameters& operator=(const EncodingParameters&) = default;
		inline EncodingParameters& operator=(const DecodingParameters& source)
		{
			DecodingParameters& ref = *this;
			ref = source;
			return *this;
		}
	};
}
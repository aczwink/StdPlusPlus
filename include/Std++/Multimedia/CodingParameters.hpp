/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Optional.hpp>
#include <Std++/Math/Size.hpp>
#include <Std++/Buffers/FixedSizeBuffer.hpp>

namespace StdXX::Multimedia
{
	struct AudioCodingParameters
	{
		uint32 sampleRate = 0;
	};

	struct VideoCodingParameters
	{
		Math::Size<uint16> size;
	};

	struct CodingParameters
	{
		DataType dataType;
		const CodingFormat *codingFormat = nullptr;
		Optional<bool> vbr;
		uint32 bitRate = 0; //if vbr then this is the average bitrate
		Optional<FixedSizeBuffer> codecPrivateData;

		AudioCodingParameters audio;
		VideoCodingParameters video;
	};
}
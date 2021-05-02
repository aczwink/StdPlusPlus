/*
* Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Definitions.h>

//FourCCs are always little endian
#define FOURCC(fcc) (uint32((fcc)[0]) | (uint32((fcc)[1]) << 8u) | (uint32((fcc)[2]) << 16u) | (uint32((fcc)[3]) << 24u))

namespace StdXX::Multimedia
{
	//TODO: Because of binary-compatibility this enum should only be internal. Users should instead use a binary-compatible coding format representation (like string, UTI? etc.)
	enum class CodingFormatId
	{
		Unknown = -1,

		//Audio codecs
		AC3,
		AAC,
		MP3,
		PCM_Float32LE,
		PCM_S16BE,
		PCM_S16LE,
		PCM_S8,
		PCM_U8,
		Vorbis,

		//Subtitle codecs
		UTF8PlainText,

		//Video codecs
		RawVideo,
		H264,
		MPEG4Part2,
		MS_MPEG4Part2V2,
		PNG,
		Theora,
	};
}
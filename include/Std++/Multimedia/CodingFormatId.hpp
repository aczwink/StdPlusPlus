/*
* Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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

/*
Mircosoft FourCCs:
http://www.faqs.org/rfcs/rfc2361.html
https://msdn.microsoft.com/de-de/library/windows/desktop/bb970509(v=vs.85).aspx
*/
//FourCCs are always little endian
#define FOURCC(fcc) (uint32((fcc)[0]) | (uint32((fcc)[1]) << 8u) | (uint32((fcc)[2]) << 16u) | (uint32((fcc)[3]) << 24u))

namespace StdXX
{
	namespace Multimedia
	{
		/*
		 * In order to not break binary-compatibility, assign each enumerator a unique value!
		 */
		enum class CodingFormatId
		{
			Unknown = -1,

			//Audio codecs
			AAC = 0xFF, //https://docs.microsoft.com/en-us/windows/desktop/medfound/audio-subtype-guids
			MP3 = 0x55,
			PCM_S16LE = 1,
			Vorbis = 0x566F, //"Vo"

			//Video codecs
			RawVideo = 0,
			H264 = FOURCC(u8"H264"),
			MS_MPEG4Part2V2 = FOURCC(u8"MP42"),
			PNG = FOURCC(u8"png "),
			Theora = FOURCC(u8"theo")
		};
	}
}
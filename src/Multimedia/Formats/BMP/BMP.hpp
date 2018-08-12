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
//Local
#include <Std++/Containers/FiniteSet/FiniteSet.hpp>
#include <Std++/Multimedia/VideoStream.hpp>
#include <Std++/Streams/InputStream.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;
//Definitions
#define BMP_HEADER_TYPE u8"BM"
#define BMP_HEADER_TYPE_SIZE 2
#define BMP_INFOHEADER_SIZE 40

//Prototypes
//void AddMS_FourCC_VideoCodecs(FiniteSet<CodecId> &refCodecSet);
namespace _stdxx_
{
	void ReadBMPHeader(bool &refIsBottomUp, InputStream &inputStream, VideoStream &stream);
	void WriteBitmapInfoHeader(VideoStream &stream, OutputStream &outputStream);
}
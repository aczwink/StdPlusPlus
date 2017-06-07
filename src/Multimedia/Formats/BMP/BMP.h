/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
//Local
#include <ACStdLib/Containers/FiniteSet/FiniteSet.hpp>
#include <ACStdLib/Multimedia/VideoStream.hpp>
#include <ACStdLib/Streams/InputStream.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Multimedia;
//Definitions
#define BMP_HEADER_TYPE "BM"
#define BMP_HEADER_TYPE_SIZE 2
#define BMP_INFOHEADER_SIZE 40

//Prototypes
void AddMS_FourCC_VideoCodecs(FiniteSet<CodecId> &refCodecSet);
void ReadBMPHeader(bool &refIsBottomUp, InputStream &inputStream, VideoStream &refStream);
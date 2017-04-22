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
#pragma once

namespace ACStdLib
{
    namespace Multimedia
    {
        enum class ECodecId
        {
            Unknown,

            //Audio
            AAC,
            AC3,
            MP2,
            MP3,
            PCM_S16LE,
            PCM_Float32LE,

            //Video
            BGR24,
            BGRA32,
            H264,
            JPEG,
            MPEG1Video,
            MPEG2Video,
            MS_MPEG4Part2,
            PNG,
            RGB24,
            S3TC_DXT1,
            S3TC_DXT5,

            //Subtitle
            DVD_Subtitle,
        };
    }
}
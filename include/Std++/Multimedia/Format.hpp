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
#include "../Containers/FiniteSet/FiniteSet.hpp"
#include "../Containers/Strings/UTF-8/UTF8String.hpp"
#include "../Definitions.h"
#include "../Streams/BufferInputStream.hpp"
#include "../Streams/ASeekableOutputStream.h"
#include "EnumTypes.hpp"
#include "CodecId.hpp"

namespace StdPlusPlus
{
    namespace Multimedia
    {
        //Forward declarations
        class Demuxer;
        class Muxer;
        class Codec;

        struct FormatInfo
        {
            bool supportsByteSeeking;
        };

        class Format
        {
        protected:
            //Constants
            static STDPLUSPLUS_API const float32 FORMAT_MATCH_BUFFER_TOO_SMALL;

        public:
            //Destructor
            virtual ~Format(){}

            //Abstract
            virtual Demuxer *CreateDemuxer(SeekableInputStream &refInput) const = 0;
            virtual Muxer *CreateMuxer(ASeekableOutputStream &refOutput) const = 0;
            virtual CodecId GetDefaultCodec(DataType dataType) const = 0;
            virtual OldString GetExtension() const = 0;
            virtual void GetFormatInfo(FormatInfo &refFormatInfo) const = 0;
            virtual OldString GetName() const = 0;
            virtual FiniteSet<CodecId> GetSupportedCodecs(DataType dataType) const = 0;
            virtual float32 Matches(BufferInputStream &refBuffer) const = 0;

            //Functions
            static STDPLUSPLUS_API const Format *Find(SeekableInputStream &inputStream);
            static STDPLUSPLUS_API const Format *FindByExtension(const OldString &refExtension);
            static STDPLUSPLUS_API void Register(Format *pFormat);
        };
    }
}
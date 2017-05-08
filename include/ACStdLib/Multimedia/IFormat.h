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
//Local
#include "../Containers/FiniteSet/FiniteSet.hpp"
#include "ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp"
#include "../Definitions.h"
#include "../Streams/CBufferInputStream.h"
#include "../Streams/ASeekableOutputStream.h"
#include "EDataType.h"
#include "ECodecId.h"

namespace ACStdLib
{
    namespace Multimedia
    {
        //Forward declarations
        class ADemuxer;
        class AMuxer;
        class ICodec;

        struct SFormatInfo
        {
            bool supportsByteSeeking;
        };

        class IFormat
        {
        protected:
            //Constants
            static ACSTDLIB_API const float32 FORMAT_MATCH_BUFFER_TOO_SMALL;

        public:
            //Destructor
            virtual ~IFormat(){}

            //Abstract
            virtual ADemuxer *CreateDemuxer(ASeekableInputStream &refInput) const = NULL;
            virtual AMuxer *CreateMuxer(ASeekableOutputStream &refOutput) const = NULL;
            virtual ECodecId GetDefaultCodec(EDataType dataType) const = NULL;
            virtual String GetExtension() const = NULL;
            virtual void GetFormatInfo(SFormatInfo &refFormatInfo) const = NULL;
            virtual String GetName() const = NULL;
            virtual FiniteSet<ECodecId> GetSupportedCodecs(EDataType dataType) const = NULL;
            virtual float32 Matches(CBufferInputStream &refBuffer) const = NULL;

            //Functions
            static ACSTDLIB_API const IFormat *Find(ASeekableInputStream &refInput);
            static ACSTDLIB_API const IFormat *FindByExtension(const String &refExtension);
            static ACSTDLIB_API void Register(IFormat *pFormat);
        };
    }
}
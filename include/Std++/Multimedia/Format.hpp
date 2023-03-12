/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/BinaryTreeSet/BinaryTreeSet.hpp>
#include <Std++/Streams/SeekableOutputStream.hpp>
#include "../Definitions.h"
#include "../Streams/BufferInputStream.hpp"
#include "EnumTypes.hpp"
#include "CodingFormat.hpp"

namespace StdXX
{
    namespace Multimedia
    {
        //Forward declarations
        class Demuxer;
        class Muxer;

        struct FormatInfo
        {
            bool supportsByteSeeking;
        };

        class Format
        {
            friend class FormatRegistry;
        protected:
            //Constants
            static const float32 FORMAT_MATCH_BUFFER_TOO_SMALL;

        public:
            //Destructor
            virtual ~Format(){}

            //Abstract
            virtual Demuxer *CreateDemuxer(SeekableInputStream& inputStream) const = 0;
            virtual Muxer *CreateMuxer(SeekableOutputStream& outputStream) const = 0;
            virtual String GetExtension() const = 0;
            virtual void GetFormatInfo(FormatInfo& formatInfo) const = 0;
            virtual String GetName() const = 0;
            virtual DynamicArray<const CodingFormat*> GetSupportedCodingFormats(DataType dataType) const = 0;
            virtual float32 Probe(BufferInputStream &inputStream) const = 0;

            //Inline
            inline const CodingFormat* GetPreferredCodingFormat(DataType dataType) const
            {
                auto codingFormats = this->GetSupportedCodingFormats(dataType);
                if(codingFormats.IsEmpty())
                    return nullptr;
                return codingFormats[0];
            }
        };
    }
}
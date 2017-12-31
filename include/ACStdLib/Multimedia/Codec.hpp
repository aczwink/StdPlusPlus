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
#include "ACStdLib/Containers/Strings/OldString.hpp"
#include "../Definitions.h"
#include "Decoder.hpp"
#include "AParser.h"
#include "CodecId.hpp"
#include "EnumTypes.hpp"
#include "Encoder.hpp"

namespace ACStdLib
{
    namespace Multimedia
    {
        //Forward declarations
        class Stream;

        class Codec
        {
        public:
            //Destructor
            virtual ~Codec(){}

            //Methods
            virtual Decoder *CreateDecoder(Stream &refStream) const = 0;
            virtual Encoder *CreateEncoder() const = 0;
            virtual AParser *CreateParser() const = 0;
            virtual CodecId GetId() const = 0;
            virtual OldString GetName() const = 0;

            //Functions
            static ACSTDLIB_API const Codec *GetCodec(CodecId codecId);
            static ACSTDLIB_API void Register(Codec *pCodec);
        };
    }
}
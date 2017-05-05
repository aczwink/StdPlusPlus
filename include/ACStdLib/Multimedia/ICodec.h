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
#include "ACStdLib/Containers/Strings/String.h"
#include "../Definitions.h"
#include "ADecoder.h"
#include "AParser.h"
#include "ECodecId.h"
#include "EDataType.h"
#include "IEncoder.h"

namespace ACStdLib
{
    namespace Multimedia
    {
        //Forward declarations
        class AStream;

        class ICodec
        {
        public:
            //Destructor
            virtual ~ICodec(){}

            //Methods
            virtual ADecoder *CreateDecoder(AStream &refStream) const = NULL;
            virtual IEncoder *CreateEncoder() const = NULL;
            virtual AParser *CreateParser() const = NULL;
            virtual ECodecId GetId() const = NULL;
            virtual String GetName() const = NULL;

            //Functions
            static ACSTDLIB_API const ICodec *GetCodec(ECodecId codecId);
            static ACSTDLIB_API void Register(ICodec *pCodec);
        };
    }
}
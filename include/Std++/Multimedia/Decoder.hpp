/*
 * Copyright (c) 2017-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Strings/String.hpp>
#include "CodingFormatId.hpp"
#include "CodingParameters.hpp"

namespace StdXX
{
    namespace Multimedia
    {
		//Forward declarations
		class DecoderContext;

        class Decoder
        {
		public:
			//Destructor
			virtual ~Decoder() {}

			//Abstract
			virtual DecoderContext *CreateContext(DecodingParameters& decodingParameters) const = 0;
			virtual CodingFormatId GetCodingFormatId() const = 0;
			virtual String GetName() const = 0;
		};
    }
}
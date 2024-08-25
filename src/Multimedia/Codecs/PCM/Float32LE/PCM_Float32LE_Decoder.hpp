/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include "../PCM_DecoderContext.hpp"

class PCM_Float32LE_Decoder : public StdXX::Multimedia::Decoder
{
public:
	//Constructor
	inline PCM_Float32LE_Decoder()
	{
	}

	//Methods
	DecoderContext* CreateContext(DecodingParameters& decodingParameters) const override
	{
		return new PCM_DecoderContext<float32>(decodingParameters, false);
	}

	CodingFormatId GetCodingFormatId() const override
	{
		return CodingFormatId::PCM_Float32LE;
	}

	String GetName() const override
	{
		return u8"StdXX PCM Float32LE Decoder";
	}
};
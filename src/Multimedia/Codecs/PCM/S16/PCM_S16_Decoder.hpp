/*
 * Copyright (c) 2022-2024 Amir Czwink (amir130@hotmail.de)
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

class PCM_S16_Decoder : public StdXX::Multimedia::Decoder
{
public:
    //Constructor
    inline PCM_S16_Decoder(bool bigEndian) : bigEndian(bigEndian)
    {
    }

    //Methods
    DecoderContext *CreateContext(DecodingParameters& decodingParameters) const override
    {
        return new PCM_DecoderContext<int16>(decodingParameters, this->bigEndian);
    }

    CodingFormatId GetCodingFormatId() const override
    {
        if(this->bigEndian)
            return CodingFormatId::PCM_S16BE;
        return CodingFormatId::PCM_S16LE;
    }

    String GetName() const override
    {
        return u8"StdXX PCM S16 Decoder";
    }

private:
    //State
    bool bigEndian;
};
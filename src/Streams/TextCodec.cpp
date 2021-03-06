/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/Streams/TextCodec.hpp>
//Codecs
#include "Textcodecs/ASCIITextCodec.hpp"
#include "Textcodecs/Latin1Codec.hpp"
#include "Textcodecs/UTF16_TextCodec.hpp"
#include "Textcodecs/UTF8TextCodec.hpp"
#include "Textcodecs/CP437TextCodec.hpp"
//Namespaces
using namespace StdXX;

//Class functions
TextCodec *TextCodec::GetCodec(TextCodecType codecType)
{
	switch(codecType)
	{
		case TextCodecType::ASCII:
			return new ASCIITextCodec;
		case TextCodecType::CP437:
			return new _stdxx_::CP437TextCodec;
		case TextCodecType::Latin1:
			return new Latin1TextCodec;
		case TextCodecType::UTF16_BE:
			return new UTF16_TextCodec(true);
		case TextCodecType::UTF16_LE:
			return new UTF16_TextCodec(false);
		case TextCodecType::UTF8:
			return new UTF8TextCodec;
	}

	return nullptr;
}
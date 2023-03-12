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
#include <Std++/Streams/SeekableInputStream.hpp>

namespace StdXX::Typography
{
	class Typeface
	{
	public:
		//Destructor
		virtual ~Typeface() {}

		//Abstract
		virtual float32 GetAscent() const = 0;
		virtual float32 GetCapHeight() const = 0;
		virtual uint16 GetCharWidth(uint16 c) const = 0;
		virtual float32 GetDescent() const = 0;
		virtual uint32 GetGlyphIndex(uint32 codePoint) const = 0;
		virtual float32 GetMaxX() const = 0;
		virtual float32 GetMaxY() const = 0;
		virtual float32 GetMinX() const = 0;
		virtual float32 GetMinY() const = 0;
		virtual uint32 GetNumberOfGlyphs() const = 0;
		virtual String GetPostscriptName() const = 0;
		//virtual void RenderGlyph(uint32 glyphIndex, Rendering::AGeneric2DPathRenderer &refRenderer) const = 0;

		//Functions
		static Typeface *GetInstance(StdXX::SeekableInputStream &refInput);
	};
}
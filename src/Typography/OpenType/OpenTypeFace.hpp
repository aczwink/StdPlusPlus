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
#include <Std++/Typography/Typeface.hpp>
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include <Std++/Containers/Strings/String.hpp>

namespace _stdxx_
{
	class OpenTypeFace : public StdXX::Typography::Typeface
	{
	public:
		//Methods
		float32 GetAscent() const override;
		float32 GetCapHeight() const override;
		float32 GetDescent() const override;
		uint32 GetGlyphIndex(uint32 codePoint) const override;
		float32 GetMaxX() const override;
		float32 GetMaxY() const override;
		float32 GetMinX() const override;
		float32 GetMinY() const override;
		uint32 GetNumberOfGlyphs() const override;
		StdXX::String GetPostscriptName() const override;
		void ReadRequiredTables(const StdXX::BinaryTreeMap<uint32, uint32> &refTableOffsets, StdXX::SeekableInputStream &refInput);

	protected:
		//Members
		bool indexToLocFormat;

		//Methods
		void SetTableOffset(uint32 tag, const StdXX::BinaryTreeMap<uint32, uint32> &refTableOffsets, StdXX::SeekableInputStream &refInput);

	private:
		//State
		int16 ascent;
		int16 capHeight;
		int16 descent;
		StdXX::BinaryTreeMap<uint16, uint32> charMap; //maps unicode code point to glyph index
		uint16 nGlyphs;
		uint16 numberOfHMetrics;
		StdXX::String postscriptName;
		uint16 unitsPerEm;
		int16 xMin;
		int16 xMax;
		int16 yMin;
		int16 yMax;

		//Abstract
		virtual void AllocateGlyphData() = 0;
		virtual void SetGlyphWidth(uint16 glyphIndex, uint16 width) = 0;

		//Methods
		void ReadCharMaps(StdXX::SeekableInputStream &refInput);
		void ReadCharMapsFormat4(StdXX::SeekableInputStream &refInput);
		void ReadFontHeader(StdXX::SeekableInputStream &refInput);
		void ReadHorizontalHeader(StdXX::SeekableInputStream &refInput);
		void ReadHorizontalMetrics(StdXX::SeekableInputStream &refInput);
		void ReadMaximumProfile(StdXX::SeekableInputStream &refInput);
		void ReadNames(StdXX::SeekableInputStream &refInput);
		void ReadOS2Header(StdXX::SeekableInputStream &refInput);
	};
}
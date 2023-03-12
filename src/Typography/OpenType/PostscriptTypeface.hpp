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
//Local
#include "OpenTypeFace.hpp"
//Definitions
#define OPENTYPE_POSTSCRIPT_TABLETAG_CFF 0x43464620

/*
Source:
	Adobe
	The Compact Font Format Specification
	Technical Note #5176
	Version 1.0
*/

namespace _stdxx_
{
	class PostscriptTypeface : public OpenTypeFace
	{
	public:
		//Methods
		uint16 GetCharWidth(uint16 c) const override;
		uint32 GetGlyphIndex(uint32 codePoint) const override;
		void ReadPostscriptOutlineTables(const StdXX::BinaryTreeMap<uint32, uint32> &refTableOffsets, StdXX::SeekableInputStream &refInput);

	private:
		//Methods
		void AllocateGlyphData() override;
		void ReadIndex(StdXX::SeekableInputStream &refInput);
		uint32 ReadOffset(uint8 offsetSize, StdXX::InputStream &refInput);
		void SetGlyphWidth(uint16 glyphIndex, uint16 width) override;
	};
}
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
#include <Std++/Rendering/VectorPath/VectorPath.hpp>
#include "OpenTypeFace.hpp"

//Definitions
#define OPENTYPE_TRUETYPE_TABLETAG_GLYF 0x676C7966

namespace _stdxx_
{
	struct STrueTypeGlyphData
	{
		uint16 width;
		StdXX::Rendering::VectorPath *pPath;
	};

	class TrueTypeFace : public OpenTypeFace
	{
	public:
		//Destructor
		~TrueTypeFace();

		//Methods
		uint16 GetCharWidth(uint16 c) const override;
		void ReadTrueTypeOutlineTables(const StdXX::BinaryTreeMap<uint32, uint32> &refTableOffsets, StdXX::SeekableInputStream &refInput);
		//void RenderGlyph(uint32 glyphIndex, Rendering::AGeneric2DPathRenderer &refRenderer) const;

	private:
		//State
		STrueTypeGlyphData *pGlyphs;

		//Methods
		void AllocateGlyphData() override;
		void ReadGlyphOutline(uint16 glyphIndex, StdXX::SeekableInputStream &refInput);
		void SetGlyphWidth(uint16 glyphIndex, uint16 width) override;
	};
}
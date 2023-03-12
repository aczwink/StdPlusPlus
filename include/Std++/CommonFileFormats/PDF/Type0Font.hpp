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
#include <Std++/Typography/Typeface.hpp>
#include <Std++/Streams/InputStream.hpp>
#include "Font.hpp"
#include "CIDFontType2.hpp"
#include "UnicodeMap.hpp"

namespace StdXX::CommonFileFormats::PDF
{
	class Type0Font : public Font
	{
	public:
		//Constructor
		Type0Font(Typography::Typeface *pTypeface, SeekableInputStream &refInput);

		//Methods
		float32 GetAscent() const override;
		float32 GetCapHeight() const override;
		uint16 GetCharWidth(uint16 c) const;
		float32 GetCharWidth(uint16 c, float32 fontSize) const;
		float32 GetDescent() const override;
		uint16 GetGlyphIndex(uint16 c) override;
		float32 GetMaxX() const override;
		float32 GetMaxY() const override;
		float32 GetMinX() const override;
		float32 GetMinY() const override;
		String GetName() const override;
		void Visit(DocumentTreeVisitor &refVisitor) override;

	private:
		//Members
		Typography::Typeface *pTypeface;
		CIDFontType2 *pCharMapFont;
		UnicodeMap uniCodeMap;

		//Methods
		String GetType() const override;
		void WriteAdditional(OutputStream &refOutput) override;
	};
}
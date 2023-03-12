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
//Class header
#include <Std++/CommonFileFormats/PDF/Type0Font.hpp>
//Local
#include <Std++/CommonFileFormats/PDF/IndirectReference.hpp>
#include <Std++/Streams/Writers/TextWriter.hpp>
#include <Std++/Typography/Font.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats::PDF;
using namespace StdXX::Typography;

//Constructor
Type0Font::Type0Font(Typeface *pTypeface, SeekableInputStream &refInput)
{
	this->pTypeface = pTypeface;
	this->pCharMapFont = new CIDFontType2(*this, refInput);
}

//Public methods
float32 Type0Font::GetAscent() const
{
	return this->pTypeface->GetAscent();
}

float32 Type0Font::GetCapHeight() const
{
	return this->pTypeface->GetCapHeight();
}

uint16 Type0Font::GetCharWidth(uint16 c) const
{
	return this->pTypeface->GetCharWidth(c);
}

float32 Type0Font::GetCharWidth(uint16 c, float32 fontSize) const
{
	Typography::Font font(*this->pTypeface, fontSize);

	return font.GetCharWidth(c);
}

float32 Type0Font::GetDescent() const
{
	return this->pTypeface->GetDescent();
}

uint16 Type0Font::GetGlyphIndex(uint16 c)
{
	uint16 glyphIndex;

	glyphIndex = this->pTypeface->GetGlyphIndex(c);
	this->uniCodeMap.Insert(c, glyphIndex);

	return glyphIndex;
}

float32 Type0Font::GetMaxX() const
{
	return this->pTypeface->GetMaxX();
}

float32 Type0Font::GetMaxY() const
{
	return this->pTypeface->GetMaxY();
}

float32 Type0Font::GetMinX() const
{
	return this->pTypeface->GetMinX();
}

float32 Type0Font::GetMinY() const
{
	return this->pTypeface->GetMinY();
}


String Type0Font::GetName() const
{
	return this->pTypeface->GetPostscriptName();
}

void Type0Font::Visit(DocumentTreeVisitor &refVisitor)
{
	refVisitor.AcceptIndirectObject(*this->pCharMapFont);
	refVisitor.AcceptIndirectObject(this->uniCodeMap);
}

//Private methods
String Type0Font::GetType() const
{
	return "Type0";
}

void Type0Font::WriteAdditional(OutputStream &refOutput)
{
	IndirectReference indirectRefDescendant(*this->pCharMapFont);
	IndirectReference indirectRefUnicodeMap(this->uniCodeMap);

	TextWriter textWriter(refOutput, TextCodecType::ASCII);

	textWriter << "/DescendantFonts [";
	indirectRefDescendant.Write(refOutput);
	textWriter << "]";

	textWriter << "/Encoding /Identity-H";
	textWriter << "/ToUnicode ";
	indirectRefUnicodeMap.Write(refOutput);
}
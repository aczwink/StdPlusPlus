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
#include <Std++/CommonFileFormats/PDF/CIDFontType2.hpp>
//Namespaces
using namespace StdXX::CommonFileFormats::PDF;

//TODO: reimplement this. as reference see this: https://github.com/itext/itext7/blob/develop/kernel/src/main/java/com/itextpdf/kernel/font/PdfType0Font.java

//Constructor
CIDFontType2::CIDFontType2(Type0Font &refParent, SeekableInputStream &refInput) : refParent(refParent), fontDescriptor(*this, refInput)
{
	//TODO: source code lost :(
}

//Public methods
float32 CIDFontType2::GetAscent() const
{
	//TODO: source code lost :(
	return 0;
}

float32 CIDFontType2::GetCapHeight() const
{
	//TODO: source code lost :(
	return 0;
}

uint16 CIDFontType2::GetCharWidth(uint16 c) const
{
	//TODO: source code lost :(
}

float32 CIDFontType2::GetCharWidth(uint16 c, float32 fontSize) const
{
	//TODO: source code lost :(
}

float32 CIDFontType2::GetDescent() const
{
	//TODO: source code lost :(
	return 0;
}

uint16 CIDFontType2::GetGlyphIndex(uint16 c)
{
	//TODO: source code lost :(
}

float32 CIDFontType2::GetMaxX() const
{
	//TODO: source code lost :(
	return 0;
}

float32 CIDFontType2::GetMaxY() const
{
	//TODO: source code lost :(
	return 0;
}

float32 CIDFontType2::GetMinX() const
{
	//TODO: source code lost :(
	return 0;
}

float32 CIDFontType2::GetMinY() const
{
	//TODO: source code lost :(
	return 0;
}

StdXX::String CIDFontType2::GetName() const
{
	//TODO: source code lost :(
}

void CIDFontType2::Visit(DocumentTreeVisitor &refVisitor)
{
	//TODO: source code lost :(
}

StdXX::String CIDFontType2::GetType() const
{
	//TODO: source code lost :(
}

void CIDFontType2::WriteAdditional(StdXX::OutputStream &refOutput)
{
	//TODO: source code lost :(
}
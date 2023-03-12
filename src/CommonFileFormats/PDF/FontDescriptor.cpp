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
#include <Std++/CommonFileFormats/PDF/FontDescriptor.hpp>
//Local
#include <Std++/CommonFileFormats/PDF/IndirectReference.hpp>
#include <Std++/Streams/Writers/TextWriter.hpp>
//Namespaces
using namespace StdXX::CommonFileFormats::PDF;

//Constructor
FontDescriptor::FontDescriptor(Font &refFont, SeekableInputStream &refInput) : refFont(refFont)
{
	this->fontFile.buffer.EnsureCapacity(refInput.QueryRemainingBytes());
	refInput.FlushTo(this->fontFile.buffer);
}

//Public methods
void FontDescriptor::Visit(DocumentTreeVisitor &refVisitor)
{
	refVisitor.AcceptIndirectObject(this->fontFile);
}

void FontDescriptor::Write(OutputStream &outputStream)
{
	IndirectReference indirectRef(this->fontFile);

	TextWriter textWriter(outputStream, TextCodecType::ASCII);

	textWriter << "<<";
	textWriter << "/Type/FontDescriptor";
	textWriter << "/Ascent " << this->refFont.GetAscent();
	textWriter << "/CapHeight " << this->refFont.GetCapHeight();
	textWriter << "/Descent " << this->refFont.GetDescent();
	textWriter << "/Flags 32";//TODO
	textWriter << "/FontBBox [" << this->refFont.GetMinX() << ' ' << this->refFont.GetMinY() << ' ' << this->refFont.GetMaxX() << ' ' << this->refFont.GetMaxY() << ']';
	textWriter << "/FontFile2 ";
	indirectRef.Write(outputStream);
	textWriter << "/FontName/SUBSET+" << this->EncodeName(this->refFont.GetName());
	textWriter << "/ItalicAngle 0";//TODO
	textWriter << "/StemV 80";//TODO
	textWriter << ">>\n";
}

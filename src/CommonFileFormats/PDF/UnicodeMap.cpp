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
#include <Std++/CommonFileFormats/PDF/UnicodeMap.hpp>
//Local
#include <Std++/Streams/Writers/TextWriter.hpp>
//Namespaces
using namespace StdXX::CommonFileFormats::PDF;

//Public methods
void UnicodeMap::Visit(DocumentTreeVisitor &refVisitor)
{
}

void UnicodeMap::Write(OutputStream &outputStream)
{
	OutputStream &refContent = this->buffer;

	TextWriter textWriter(refContent, TextCodecType::ASCII);

	textWriter << "/CIDInit /ProcSet findresource begin\n";
	textWriter << "12 dict begin\n";
	textWriter << "begincmap\n";
	textWriter << "/CMapType 2 def\n";
	textWriter << "/CMapName/R" << this->objectNumber << " def\n";
	textWriter << "1 begincodespacerange\n";
	textWriter << "<0000><ffff>\n";
	textWriter << "endcodespacerange\n";

	textWriter << this->codePoints.GetNumberOfElements() << " beginbfrange\n";
	for(const auto& kv : this->codePoints)
	{
		String source = String::HexNumber(kv.value, 4, false);
		textWriter << u8'<' << source << u8'>';
		textWriter << u8'<' << source << u8'>';
		textWriter << u8'<' << String::HexNumber((uint32)kv.key, 4, false) << ">\n";
	}
	textWriter << "endbfrange\n";
	textWriter << "endcmap\n";
	textWriter << "CMapName currentdict /CMap defineresource pop\n";
	textWriter << "end end\n";

	StreamObject::Write(outputStream);
}

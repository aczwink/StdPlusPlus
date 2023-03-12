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
#include <Std++/CommonFileFormats/PDF/Font.hpp>
//Local
#include <Std++/Streams/Writers/TextWriter.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats::PDF;

//Public methods
void Font::Write(OutputStream &outputStream)
{
	TextWriter textWriter(outputStream, TextCodecType::ASCII);

	textWriter << "<<";
	textWriter << "/Type /Font";
	textWriter << "/Subtype /" << this->GetType();
	textWriter << "/BaseFont /SUBSET+" << this->EncodeName(this->GetName());
	//refOutput << "/BaseFont /Times-Roman";

	this->WriteAdditional(outputStream);

	textWriter << ">>\n";
}

//Private methods
void Font::WriteAdditional(OutputStream &refOutput)
{
}
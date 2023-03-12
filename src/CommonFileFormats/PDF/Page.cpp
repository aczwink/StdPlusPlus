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
#include <Std++/CommonFileFormats/PDF/Page.hpp>
//Local
#include <Std++/CommonFileFormats/PDF/IndirectReference.hpp>
#include <Std++/CommonFileFormats/PDF/PageTree.hpp>
#include <Std++/Streams/Writers/TextWriter.hpp>
//Namespaces
using namespace StdXX::CommonFileFormats::PDF;

//Constructor
Page::Page(PageTree& parent) : parent(parent)
{
	this->width = 595.27559f; //roughly A4
	this->height = 841.88976f; //mm to userunit -> mm * 72 / 25.4 on 72dpi
}

//Public methods
void Page::Visit(DocumentTreeVisitor &refVisitor)
{
	refVisitor.AcceptIndirectObject(this->content);
}

void Page::Write(OutputStream &outputStream)
{
	IndirectReference parent(this->parent);
	IndirectReference contentRef(this->content);

	TextWriter textWriter(outputStream, TextCodecType::ASCII);

	textWriter << "<</Type /Page /Parent ";
	parent.Write(outputStream);
	textWriter << " /Contents ";
	contentRef.Write(outputStream);
	textWriter << " /Resources<</ProcSet [/PDF /Text] /Font <<";
	/*for(auto it = this->u)
	foreach(it, this->usedFonts)
	{
		CIndirectReference fontRef(*(*it));

		refOutput << "/F" << it.GetIndex() << ' ';
		fontRef.Write(refOutput);
	}*/
	textWriter << ">>>> /MediaBox [0 0 " << this->width << ' ' << this->height << "]>>\n";
}
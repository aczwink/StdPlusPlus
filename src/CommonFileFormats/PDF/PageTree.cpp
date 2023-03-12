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
#include <Std++/CommonFileFormats/PDF/PageTree.hpp>
//Local
#include <Std++/Streams/Writers/TextWriter.hpp>
#include <Std++/CommonFileFormats/PDF/IndirectReference.hpp>
//Namespaces
using namespace StdXX::CommonFileFormats::PDF;

//Destructor
PageTree::~PageTree()
{
	for(const auto& page : this->pages)
	{
		delete page;
	}
}

//Public methods
Page &PageTree::AddPage()
{
	Page *pPage = new Page(*this);
	this->pages.InsertTail(pPage);

	return *pPage;
}

void PageTree::Visit(DocumentTreeVisitor& visitor)
{
	for(Page *const& refpPage : this->pages)
	{
		visitor.AcceptIndirectObject(*refpPage);
	}
}

void PageTree::Write(OutputStream &outputStream)
{
	TextWriter textWriter(outputStream, TextCodecType::ASCII);

	textWriter << "<</Type /Pages /Count " << this->pages.GetNumberOfElements();
	textWriter << " /Kids [";

	for(const auto& page : this->pages)
	{
		IndirectReference ref(*page);

		ref.Write(outputStream);
	}

	textWriter << "]>>\n";
}
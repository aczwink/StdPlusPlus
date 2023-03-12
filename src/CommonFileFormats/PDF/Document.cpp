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
#include <Std++/CommonFileFormats/PDF/Document.hpp>
//Local
#include <Std++/Streams/Writers/TextWriter.hpp>
#include <Std++/Streams/Writers/DataWriter.hpp>
#include <Std++/CommonFileFormats/PDF/IndirectReference.hpp>
#include <Std++/CommonFileFormats/PDF/IntObject.hpp>
#include <Std++/CommonFileFormats/PDF/Dictionary.hpp>
#include <Std++/Streams/FileInputStream.hpp>
#include <Std++/CommonFileFormats/PDF/Type0Font.hpp>
#include <Std++/Typography/Typeface.hpp>
//Namespaces
using namespace StdXX::CommonFileFormats::PDF;

//Public methods
Font *Document::AddFont(const FileSystem::Path& fontPath)
{
	FileInputStream input(fontPath);

	Typography::Typeface* pTypeface = Typography::Typeface::GetInstance(input);
	input.SeekTo(0);
	Font* pFont = new Type0Font(pTypeface, input);
	this->fonts.InsertTail(pFont);

	return pFont;
}

void Document::CreateXRefFromCatalog()
{
	class XRefCreator : public DocumentTreeVisitor
	{
	public:
		//Constructor
		inline XRefCreator(CrossReferenceTable& xref) : xref(xref)
		{
			this->currentObjectNumber = 1;
		}

		//Methods
		void AcceptIndirectObject(IndirectObject& indirectObject)
		{
			indirectObject.objectNumber = this->currentObjectNumber++;
			this->xref.AddEntry(indirectObject);

			indirectObject.Visit(*this);
		}

	private:
		//State
		uint32 currentObjectNumber;
		CrossReferenceTable& xref;
	};

	this->xref = {}; //reset it

	XRefCreator creator(this->xref);
	//Visit all document subnodes
	creator.AcceptIndirectObject(this->catalog);

	for(const auto& font : this->fonts)
	{
		creator.AcceptIndirectObject(*font);
	}
}

void Document::Write(SeekableOutputStream& outputStream)
{
	this->WriteHeader(outputStream);

	uint32 xrefOffset = this->xref.Write(outputStream);
	this->WriteTrailer(outputStream, xrefOffset);
}

//Private methods
void Document::WriteHeader(OutputStream& outputStream)
{
	TextWriter textWriter(outputStream, TextCodecType::ASCII);
	DataWriter dataWriter(true, outputStream);

	//7.5.2
	textWriter << "%PDF-1.4\n";
	dataWriter.WriteByte('%');
	dataWriter.WriteByte(128);
	dataWriter.WriteByte(128);
	dataWriter.WriteByte(128);
	dataWriter.WriteByte(128);
	dataWriter.WriteByte('\n');
}

void Document::WriteTrailer(SeekableOutputStream& outputStream, uint32 xrefOffset)
{
	Dictionary dict;

	//7.5.5 File Trailer
	dict.Insert("Size", new IntObject(this->xref.NumberOfEntries()+1));
	dict.Insert("Root", new IndirectReference(this->catalog));

	TextWriter textWriter(outputStream, TextCodecType::ASCII);

	textWriter.WriteString("trailer\n");
	dict.Write(outputStream);
	textWriter.WriteString("startxref\n");
	textWriter << xrefOffset << u8'\n';
	textWriter << "%%EOF\n";
}
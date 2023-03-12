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
#include <Std++/CommonFileFormats/PDF/Graphics.hpp>
//Local
#include <Std++/CommonFileFormats/PDF/Font.hpp>
#include <Std++/CommonFileFormats/PDF/Page.hpp>
#include "LanguageProcessor.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX::CommonFileFormats::PDF;

//Public methods
void Graphics::SetFont(Font *pFont, float32 fontSize)
{
	uint32 fontIdx;

	this->pCurrentFont = pFont;
	fontIdx = this->refPage.AddFont(pFont);
	this->textWriter << "/F" << fontIdx << u8' ' << fontSize << " Tf\n";
}

void Graphics::ShowText(const String &refString)
{
	if(!refString.IsEmpty())
	{
		uint16 i, j, k, codePoint;
		DynamicArray<uint16> outputChars;

		i = 0;

		this->textWriter << u8'[';
		while(i < refString.GetLength())
		{
			LanguageProcessor &refProc = LanguageProcessor::GetInstance(refString.GetRawData()[i]);

			//get the text portion that is valid to be processed by this processor
			for(j = i; j < refString.GetLength() && (refProc.IsInLanguage(refString.GetRawData()[j]) || this->IsWhitespace(refString.GetRawData()[j])); j++);

			//process the portion
			outputChars.Release();
			refProc.Process((uint16 *)&refString.GetRawData()[i], j - i, outputChars);

			//write output
			this->textWriter << u8'<';
			for(k = 0; k < outputChars.GetNumberOfElements(); k++)
			{
				codePoint = this->pCurrentFont->GetGlyphIndex(outputChars[k]);

				this->textWriter << String::HexNumber((uint32)codePoint, 4, false);
			}
			this->textWriter << u8'>';

			i = j;
		}
		this->textWriter << "]TJ\n";
	}
}

//Private methods
bool Graphics::IsWhitespace(uint16 c)
{
	switch(c)
	{
		case 32: //space
			return true;
	}

	return false;
}
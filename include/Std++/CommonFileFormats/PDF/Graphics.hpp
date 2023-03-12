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
#pragma once
//Local
#include <Std++/Streams/Writers/TextWriter.hpp>

namespace StdXX::CommonFileFormats::PDF
{
	//Forward declarations
	class Font;
	class Page;

	class Graphics
	{
	public:
		//Constructor
		inline Graphics(OutputStream& refRenderTarget, Page &refPage) : textWriter(refRenderTarget, TextCodecType::ASCII), refPage(refPage)
		{
			this->SaveState();
		}

		//Destructor
		inline ~Graphics()
		{
			this->RestoreState();
		}

		//Methods
		void SetFont(Font* pFont, float32 fontSize);
		void ShowText(const String &refString);

		//Inline
		inline void BeginText()
		{
			this->textWriter << "BT\n";
		}

		inline void ClosePath()
		{
			this->textWriter << "h\n";
		}

		inline void ConcatToCTM(float32 a, float32 b, float32 c, float32 d, float32 e, float32 f)
		{
			this->textWriter << a << ' ' << b << ' ' << c << ' ' << d << ' ' << e << ' ' << f << " cm\n";
		}

		inline void EndText()
		{
			this->textWriter << "ET\n";
		}

		inline void Fill()
		{
			this->textWriter << "f\n";
		}

		inline void LineTo(float32 x, float32 y)
		{
			this->textWriter << x << ' ' << y << " l\n";
		}

		inline void MoveTextPos(float32 x, float32 y)
		{
			this->textWriter << x << ' ' << y << " Td\n";
		}

		inline void MoveTo(float32 x, float32 y)
		{
			this->textWriter << x << ' ' << y << " m\n";
		}

		inline void Rectangle(float32 x, float32 y, float32 w, float32 h)
		{
			this->textWriter << x << ' ' << y << ' ' << w << ' ' << h << " re\n";
		}

		inline void RestoreState()
		{
			this->textWriter << "Q\n";
		}

		inline void SaveState()
		{
			this->textWriter << "q\n";
		}

		inline void SetGrayStroke(float32 grayStroke)
		{
			this->textWriter << grayStroke << " G\n";
		}

		inline void SetLineCap(uint8 style)
		{
			ASSERT_TRUE(Math::IsValueInInterval(style, 0_u8, 2_u8));
			this->textWriter << (uint32)style << " J\n";
		}

		inline void SetLineJoin(uint8 style)
		{
			ASSERT_TRUE(Math::IsValueInInterval(style, 0_u8, 2_u8));
			this->textWriter << (uint32)style << " j\n";
		}

		inline void SetLineWidth(float32 w)
		{
			this->textWriter << w << " w\n";
		}

		inline void SetTextMatrix(float32 a, float32 b, float32 c, float32 d, float32 e, float32 f)
		{
			this->textWriter << a << ' ' << b << ' ' << c << ' ' << d << ' ' << e << ' ' << f << " Tm\n";
		}

		inline void ShowSimpleText(const String &refString)
		{
			this->textWriter << '(' << refString << ")Tj\n";
		}

		inline void Stroke()
		{
			this->textWriter << "S\n";
		}

	private:
		//State
		TextWriter textWriter;
		Page &refPage;
		Font *pCurrentFont;

		//Methods
		bool IsWhitespace(uint16 c);
	};
}
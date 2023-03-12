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
//Local
#include "../LanguageProcessor.hpp"

/*
Unicode tables:
http://en.wikipedia.org/wiki/Arabic_%28Unicode_block%29
http://en.wikipedia.org/wiki/Arabic_Presentation_Forms-B
*/
namespace _stdxx_
{
	class ArabicLanguageProcessor : public LanguageProcessor
	{
	public:
		bool IsInLanguage(uint16 c) const override;
		void Process(uint16 *pInput, uint16 nInputChars, StdXX::DynamicArray<uint16> &refOutChars) const override;

	private:
		//Methods
		uint16 GetLigature(uint16 *pInput, uint16 nLeftChars, bool &refConnectWithPrevious, uint16 &refPresentationChar) const;
		bool GetsConnected(uint16 c) const;
		bool IsLetter(uint16 c) const;
	};
}
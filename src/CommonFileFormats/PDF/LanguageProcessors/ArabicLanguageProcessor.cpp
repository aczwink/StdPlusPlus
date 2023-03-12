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
#include "ArabicLanguageProcessor.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
//Definitions
#define CHAR_NONE 0
#define CHAR_ALEF_HAMZA_ABOVE 0x623
#define CHAR_ALEF 0x627
#define CHAR_BA 0x628
#define CHAR_TA_MARBUTA 0x629
#define CHAR_TA 0x62A
#define CHAR_JIM 0x62C
#define CHAR_HHA 0x62D
#define CHAR_KHA 0x62E
#define CHAR_DAL 0x62F
#define CHAR_RA 0x631
#define CHAR_SIN 0x633
#define CHAR_SCHIN 0x634
#define CHAR_SAAD 0x635
#define CHAR_AEIN 0x639
#define CHAR_QAF 0x642
#define CHAR_LAM 0x644
#define CHAR_MIM 0x645
#define CHAR_NUN 0x646
#define CHAR_WAW 0x648
#define CHAR_ALEF_MAKSURA 0x649
#define CHAR_YA 0x64A

static const uint16 g_charTable[][4] =
{
		//First column = Isolated(also form in input string)
		//Second column = Initial
		//Third column = Medial
		//Fourth column = Final
		{CHAR_ALEF_HAMZA_ABOVE,      0,      0,  0xFE84},
		{                   80, 80, 80, 80}, //0x624
		{                   80, 80, 80, 80}, //0x625
		{                   80, 80, 80, 80}, //0x626
		{            CHAR_ALEF,      0,      0,  0xFE8E},
		{              CHAR_BA, 0xFE91, 0xFE92, CHAR_BA},
		{      CHAR_TA_MARBUTA,     80,     80,  0xFE94},
		{              CHAR_TA, 0xFE97,     80,      80},
		{                   80, 0xFE9B, 0xFE9C,      80}, //tha
		{             CHAR_JIM,     80, 0xFEA0,      80},
		{             CHAR_HHA,     80, 0xFEA4,      80},
		{             CHAR_KHA, 0xFEA7,     80,      80},
		{             CHAR_DAL,     80,     80,  0xFEAA},
		{                   80, 80, 80, 80}, //0x630
		{              CHAR_RA,     80,     80,  0xFEAE},
		{                   80, 80, 80, 80}, //0x632
		{             CHAR_SIN, 0xFEB3, 0xFEB4,      80},
		{           CHAR_SCHIN, 0xFEB7,     80,      80},
		{            CHAR_SAAD,     80, 0xFEBC,      80},
		{                   80, 80, 80, 80}, //0x636
		{                   80, 80, 80, 80}, //0x637
		{                   80, 80, 80, 80}, //0x638
		{            CHAR_AEIN, 0xFECB, 0xFECC,      80},
		{                   80, 80, 80, 80}, //0x63A
		{                   80, 80, 80, 80}, //0x63B
		{                   80, 80, 80, 80}, //0x63C
		{                   80, 80, 80, 80}, //0x63D
		{                   80, 80, 80, 80}, //0x63E
		{                   80, 80, 80, 80}, //0x63F
		{                   80, 80, 80, 80}, //0x640
		{                   80, 80, 80, 80}, //0x641
		{             CHAR_QAF,     80, 0xFED8,      80},
		{                   80, 80, 80, 80}, //0x643
		{             CHAR_LAM, 0xFEDF, 0xFEE0,      80},
		{             CHAR_MIM, 0xFEE3, 0xFEE4,  0xFEE2},
		{             CHAR_NUN, 0xFEE7,     80,      80},
		{                   80, 80, 80, 80}, //0x647
		{             CHAR_WAW,     80,     80,      80},
		{    CHAR_ALEF_MAKSURA,     80,     80,  0xFEF0},
		{              CHAR_YA,     80, 0xFEF4,  0xFEF2},
};

bool ArabicLanguageProcessor::IsInLanguage(uint16 c) const
{
	return this->IsLetter(c);
}

void ArabicLanguageProcessor::Process(uint16 *pInput, uint16 nInputChars, DynamicArray<uint16> &refOutChars) const
{
	bool connectWithPrevious;
	uint16 nConsumedChars, presentationChar;
	int32 i;
	DynamicArray<uint16> presentationChars;

	connectWithPrevious = false;
	while(nInputChars)
	{
		nConsumedChars = this->GetLigature(pInput, nInputChars, connectWithPrevious, presentationChar);

		pInput += nConsumedChars;
		nInputChars -= nConsumedChars;
		presentationChars.Push(presentationChar);
	}

	//we have RTL, so turn around
	for(i = presentationChars.GetNumberOfElements()-1; i >= 0; i--)
	{
		refOutChars.Push(presentationChars[i]);
	}
}

//Private methods
uint16 ArabicLanguageProcessor::GetLigature(uint16 *pInput, uint16 nLeftChars, bool &refConnectWithPrevious, uint16 &refPresentationChar) const
{
	uint8 column;
	uint16 currentChar, nextChar;

	currentChar = *pInput;
	if(nLeftChars == 0) //no following char
		nextChar = CHAR_NONE;
	else
		nextChar = *(pInput+1);

	//check for special ligatures
	switch(currentChar)
	{
		case CHAR_LAM:
		{
			//check for lam alef
			switch(nextChar)
			{
				case CHAR_ALEF_HAMZA_ABOVE:
				{
					refConnectWithPrevious = false;
					refPresentationChar = 0xFEF7;
					return 2;
				}
					break;
			}
		}
			break;
	}

	//default ligature rules
	if(refConnectWithPrevious)
	{
		//middle or final form
		if(this->GetsConnected(currentChar) && this->IsLetter(nextChar))
		{
			//middle form
			column = 2;
		}
		else
		{
			//final form
			column = 3;
			refConnectWithPrevious = false;
		}
	}
	else
	{
		//isolated or initial form
		if(this->GetsConnected(currentChar) && this->IsLetter(nextChar))
		{
			//initial form
			column = 1;
			refConnectWithPrevious = true;
		}
		else
		{
			//isolated form
			column = 0;
			refConnectWithPrevious = false;
		}
	}

	if(this->IsLetter(currentChar))
		refPresentationChar = g_charTable[currentChar - CHAR_ALEF_HAMZA_ABOVE][column];
	else
		refPresentationChar = currentChar; //should only be white space

	return 1;
}

bool ArabicLanguageProcessor::GetsConnected(uint16 c) const
{
	switch(c)
	{
		case CHAR_NONE:
		case 32: //whitespace
		case CHAR_ALEF_HAMZA_ABOVE:
		case CHAR_ALEF:
		case CHAR_RA:
		case CHAR_WAW:
			return false;
	}

	return true;
}

bool ArabicLanguageProcessor::IsLetter(uint16 c) const
{
	return Math::IsValueInInterval(c, 0x600_u16, 0x6FF_u16);
}
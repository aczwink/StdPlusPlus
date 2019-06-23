/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Compression/HuffmanDecoder.hpp>
#include <Std++/Streams/Bitstreams/BitInputStreamBitReversed.hpp>
#include <Std++/Debug.hpp>
//Namespaces
using namespace StdXX;

//Constants
const int16 STATE_INVALID = 0; //0 is fine because no node can have the root as child

/*
This algorithm maps a tree structure into an array in the following way:
-Every node is described by two adjacent array indices (left child and right child)
-If a node is in state STATE_INVALID, it is not existent
-If a node value is a positive number it is a non-leaf
-If a node value is a negative number it is a leaf and the value is the bitwise complement of the symbol in the huffman tree

Example:
       o
    /     \
   o       o
  / \     / \
'a' 'b' 'c' 'd'

The tree is stored in the array as follows (2, 4, ~'a', ~'b', ~'c', ~'d').
The roots children are kept at index 0 and 1 respectively.
Those are internal nodes, while the first one has its children at index 1 and the other at index 4.
*/

//Constructor
HuffmanDecoder::HuffmanDecoder(uint16 nCodeWords)
{
	this->pNodes = (int16 *)MemAlloc(2 * nCodeWords * sizeof(*this->pNodes));

	//allocate root node
	this->nextAllocationIndex = 0;
	this->Allocate();
}

//Public methods
void HuffmanDecoder::AddSymbol(uint16 symbol, uint16 codeWord, uint8 bitLength)
{
	uint32 index;

	codeWord <<= 16 - bitLength;

	index = 0;
	while(bitLength > 1)
	{
		if(codeWord & 0x8000) //test highest bit
			index++;

		ASSERT(this->pNodes[index] >= 0, u8"TODO: do this correctly"); //we should never hit a leaf

		if(this->pNodes[index] > 0)
		{
			//a node is already allocated... just follow
			index = this->pNodes[index];
		}
		else
		{
			//allocate a node
			this->pNodes[index] = this->Allocate();
			index = this->pNodes[index];
		}

		codeWord <<= 1;
		bitLength--;
	}

	//we put a leaf here
	if(codeWord & 0x8000)
		index++;

	ASSERT(this->pNodes[index] == STATE_INVALID, u8"TODO: do this correctly");

	this->pNodes[index] = ~symbol;
}

void HuffmanDecoder::ConstructCanonical(const uint8 *pBitLengths, uint16 nBitLengths)
{
	uint8 bitLength, maxBitLength;
	uint16 i, codeWord;

	//get max length
	maxBitLength = 0;
	for(i = 0; i < nBitLengths; i++)
	{
		if(pBitLengths[i] > maxBitLength)
			maxBitLength = pBitLengths[i];
	}

	ASSERT(maxBitLength < 16, u8"TODO: do this correctly");

	//add symbols
	codeWord = 0;
	for(bitLength = 1; bitLength <= maxBitLength; bitLength++)
	{
		for(i = 0; i < nBitLengths; i++)
		{
			if(pBitLengths[i] == bitLength)
			{
				this->AddSymbol(i, codeWord, bitLength);

				codeWord++;
			}
		}

		codeWord <<= 1;
	}
}

uint16 HuffmanDecoder::Decode(BitInputStreamBitReversed &refBitInput) const
{
	uint32 index;

	index = 0;
	while(true)
	{
		if(refBitInput.Read(1))
			index++;

		ASSERT(this->pNodes[index] != STATE_INVALID, u8"TODO: do this correctly");

		if(this->pNodes[index] < 0)
			return ~this->pNodes[index];

		index = this->pNodes[index];
	}
}

//Private methods
uint16 HuffmanDecoder::Allocate()
{
	uint16 index;

	index = this->nextAllocationIndex;
	this->nextAllocationIndex += 2;

	this->pNodes[index] = STATE_INVALID;
	this->pNodes[index + 1] = STATE_INVALID;

	return index;
}
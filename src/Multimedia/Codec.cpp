/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/Codec.hpp>
//Namespaces
using namespace StdPlusPlus;
using namespace StdPlusPlus::Multimedia;

//Global variables
DynamicArray<const Codec *> g_codecs;

//Class functions
const Codec *Codec::GetCodec(CodecId codecId)
{
	for(const Codec *const& refpCodec : g_codecs)
	{
		if(refpCodec->GetId() == codecId)
			return refpCodec;
	}

	return nullptr;
}

void Codec::Register(Codec *pCodec)
{
	g_codecs.Push(pCodec);
}
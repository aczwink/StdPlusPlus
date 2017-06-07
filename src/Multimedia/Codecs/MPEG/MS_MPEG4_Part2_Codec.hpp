/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "../LibAV_VideoCodec.hpp"

class MS_MPEG4_Part2_Codec : public LibAV_VideoCodec
{
public:
	//Methods
	CodecId GetId() const
	{
		return CodecId::MS_MPEG4Part2;
	}

	String GetName() const
	{
		return "Microsoft MPEG-4 Part2";
	}
};
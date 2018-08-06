/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include "../ExternalVideoCodec.hpp"

class H264_Codec : public ExternalVideoCodec
{
public:
	//Methods
	CodecId GetId() const
	{
		return CodecId::H264;
	}

	String GetName() const
	{
		return u8"H.264 / MPEG-4 Part 10, Advanced Video Coding (MPEG-4 AVC)";
	}
};
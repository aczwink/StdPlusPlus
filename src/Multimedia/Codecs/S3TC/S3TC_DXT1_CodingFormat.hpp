/*
 * Copyright (c) 2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/CodingFormat.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

class S3TC_DXT1_CodingFormat : public CodingFormat
{
public:
	//Methods
	CodingFormatId GetId() const override
	{
		return CodingFormatId::S3TC_DXT1;
	}

	String GetName() const override
	{
		return "S3TC DXT1 - S3 Texture Compression";
	}
};
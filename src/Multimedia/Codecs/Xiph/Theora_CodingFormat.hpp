/*
 * Copyright (c) 2018-2024 Amir Czwink (amir130@hotmail.de)
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

class Theora_CodingFormat : public CodingFormat
{
public:
	//Methods
	CodingFormatId GetId() const override
	{
		return CodingFormatId::Theora;
	}

	String GetName() const override
	{
		return u8"Theora";
	}

	FeatureDescriptor<AudioSampleType> GetSupportedSampleTypes() const override
	{
		NOT_IMPLEMENTED_ERROR; //TODO: implement me
		return FeatureDescriptor<AudioSampleType>();
	}
};
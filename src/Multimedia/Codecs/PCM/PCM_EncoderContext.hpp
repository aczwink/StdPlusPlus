/*
 * Copyright (c) 2017-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/EncoderContext.hpp>
#include <Std++/Streams/Writers/DataWriter.hpp>
#include <Std++/Multimedia/Stream.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

template <typename T>
class PCM_EncoderContext : public EncoderContext
{
public:
	//Constructor
	inline PCM_EncoderContext(const EncodingParameters& encodingParameters, bool bigEndian) : EncoderContext(encodingParameters), bigEndian(bigEndian)
	{
	}

	//Methods
	void Encode(const Frame &frame) override;
	void Flush() override;

private:
	//State
	bool bigEndian;

	//Inline
	template <typename U = T>
	inline Type::EnableIf_t<Type::IsSameType<U, float32>::value, AudioSampleType> GetSampleType() const
	{
		return AudioSampleType::Float;
	}

	template <typename U = T>
	inline Type::EnableIf_t<Type::IsSameType<U, int16>::value, AudioSampleType> GetSampleType() const
	{
		return AudioSampleType::S16;
	}

	template <typename U = T>
	inline Type::EnableIf_t<Type::IsSameType<U, float32>::value, void> WriteSample(float32 sample, DataWriter& dataWriter)
	{
		dataWriter.WriteFloat32(sample);
	}

	template <typename U = T>
	inline Type::EnableIf_t<Type::IsSameType<U, int16>::value, void> WriteSample(int16 sample, DataWriter& dataWriter)
	{
		dataWriter.WriteInt16(sample);
	}
};
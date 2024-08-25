/*
 * Copyright (c) 2022-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/DecoderContext.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Multimedia/AudioSampleFormat.hpp>
#include <Std++/Multimedia/Frame.hpp>

template<typename T>
class PCM_DecoderContext : public StdXX::Multimedia::DecoderContext
{
public:
    //Constructor
    inline PCM_DecoderContext(DecodingParameters& decodingParameters, bool bigEndian) : DecoderContext(decodingParameters), bigEndian(bigEndian)
    {
    }

    //Methods
    void Decode(const IPacket &packet) override
    {
        const AudioSampleFormat& sampleFormat = this->Parameters().audio.sampleFormat.Value();
        uint32 nSamplesPerChannel = packet.GetSize() / sizeof(T) / sampleFormat.nChannels;
        AudioBuffer* audioBuffer = new AudioBuffer(nSamplesPerChannel, sampleFormat);

        BufferInputStream bufferInputStream(packet.GetData(), packet.GetSize());
        DataReader dataReader(this->bigEndian, bufferInputStream);

		for(uint32 i = 0; i < nSamplesPerChannel; i++)
		{
			for(uint8 ch = 0; ch < sampleFormat.nChannels; ch++)
			{
				const auto& channelInfo = sampleFormat.channels[ch];

				byte *src = (byte *)audioBuffer->GetPlane(channelInfo.planeIndex);
				src += i * audioBuffer->GetPlaneBlockSize(channelInfo.planeIndex);
				src += channelInfo.offset;

				T *srcTyped = (T *)src;
				*srcTyped = this->ReadSample(dataReader);
			}
		}

        this->AddFrame(new Frame(audioBuffer));
    }

private:
    //State
    bool bigEndian;

    //Inline
    template <typename U = T>
	inline Type::EnableIf_t<Type::IsSameType<U, float32>::value, float32> ReadSample(DataReader& dataReader) const
	{
		return dataReader.ReadFloat32();
	}

	template <typename U = T>
    inline Type::EnableIf_t<Type::IsSameType<U, int16>::value, int16> ReadSample(DataReader& dataReader) const
	{
    	return dataReader.ReadInt16();
	}
};
/*
 * Copyright (c) 2022-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/DecoderContext.hpp>
#include <Std++/Multimedia/AudioFrame.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Multimedia/AudioSampleFormat.hpp>
#include <Std++/Multimedia/Stream.hpp>

class PCM_S16_DecoderContext : public StdXX::Multimedia::DecoderContext
{
public:
    //Constructor
    inline PCM_S16_DecoderContext(Stream &stream, bool bigEndian) : DecoderContext(stream), bigEndian(bigEndian)
    {
    }

    //Methods
    void Decode(const IPacket &packet) override
    {
        const AudioSampleFormat& sampleFormat = this->stream.codingParameters.audio.sampleFormat.Value();
        uint32 nSamplesPerChannel = packet.GetSize() / sizeof(int16) / sampleFormat.nChannels;
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

				int16 *srcTyped = (int16 *)src;
				*srcTyped = dataReader.ReadInt16();
			}
		}

        this->AddFrame(new AudioFrame(audioBuffer));
    }

private:
    //State
    bool bigEndian;
};
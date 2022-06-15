/*
 * Copyright (c) 2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/AudioStream.hpp>
#include <Std++/Multimedia/AudioFrame.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Multimedia/AudioSampleFormat.hpp>

class PCM_S16BE_DecoderContext : public StdXX::Multimedia::DecoderContext
{
public:
    //Constructor
    inline PCM_S16BE_DecoderContext(const Decoder &decoder, Stream &stream) : DecoderContext(decoder, stream)
    {
    }

    //Methods
    void Decode(const IPacket &packet) override
    {
        AudioStream& audioStream = dynamic_cast<AudioStream &>(this->stream);
        const AudioSampleFormat& sampleFormat = audioStream.sampleFormat.Value();
        uint32 nSamplesPerChannel = packet.GetSize() / sizeof(int16) / sampleFormat.nChannels;
        AudioBuffer* audioBuffer = new AudioBuffer(nSamplesPerChannel, sampleFormat);

        BufferInputStream bufferInputStream(packet.GetData(), packet.GetSize());
        DataReader dataReader(true, bufferInputStream);

        switch(sampleFormat.nChannels)
        {
            case 1:
            {
                int16* data = static_cast<int16 *>(audioBuffer->GetPlane(0));

                for(uint32 i = 0; i < nSamplesPerChannel; i++)
                    *data++ = dataReader.ReadInt16();
            }
            break;
            case 2:
            {
                ASSERT_EQUALS(2, sampleFormat.nPlanes);
                //TODO: check which channel (left or right) is on which plane
                int16* left = static_cast<int16 *>(audioBuffer->GetPlane(sampleFormat.channels[0].planeIndex));
                int16* right = static_cast<int16 *>(audioBuffer->GetPlane(sampleFormat.channels[1].planeIndex));

                for(uint32 i = 0; i < nSamplesPerChannel; i++)
                {
                    *left++ = dataReader.ReadInt16();
                    *right++ = dataReader.ReadInt16();
                }
            }
            break;
            default:
                NOT_IMPLEMENTED_ERROR; //TODO: implement me
        }

        this->AddFrame(new AudioFrame(audioBuffer));
    }
};
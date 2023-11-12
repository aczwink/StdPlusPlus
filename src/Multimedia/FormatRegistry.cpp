/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/FormatRegistry.hpp>
//Local
#include <Std++/Multimedia/Decoder.hpp>
#include <Std++/Multimedia/Encoder.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;
//Definitions
#define DETECTIONBUFFER_MINSIZE 64
#define DETECTIONBUFFER_MAXSIZE 64u

//Local functions
static void SkipID3(SeekableInputStream &inputStream)
{
    byte id3Identifier[3];
    uint32 totalTagSize;

    inputStream.ReadBytes(id3Identifier, 3);
    if(MemCmp(id3Identifier, "ID3", 3) == 0)
    {
        inputStream.Skip(3);

        DataReader reader(true, inputStream);
        totalTagSize = ((reader.ReadByte() & 0x7F) << 21) | ((reader.ReadByte() & 0x7F) << 14) | ((reader.ReadByte() & 0x7F) << 7) | (reader.ReadByte() & 0x7F);

        inputStream.Skip(totalTagSize);
    }
    else
    {
        inputStream.Rewind(3);
    }
}

//Constructor
FormatRegistry::FormatRegistry()
{
    ShutdownManager::Instance().Register(this);
    this->RegisterCodingFormats();
    this->RegisterContainerFormats();
    this->RegisterDecoders();
    this->RegisterEncoders();
}

//Public methods
const CodingFormat *FormatRegistry::FindCodingFormatById(CodingFormatId codingFormatId)
{
    if(this->codingFormats.Contains(codingFormatId))
        return this->codingFormats[codingFormatId].operator->();

    return nullptr;
}

const Format* FormatRegistry::FindFormatByFileExtension(const String& extension)
{
    for(const UniquePointer<Format>& format : FormatRegistry::Instance().ContainerFormats())
    {
        if(format->GetExtension() == extension)
            return format.operator->();
    }

    return nullptr;
}

const Format* FormatRegistry::ProbeFormat(SeekableInputStream &inputStream)
{
    bool resize;
    byte *pDetectionBuffer;
    uint32 detectionBufferSize, nReadBytes;
    uint64 currentOffset;
    float32 matchScore, bestScore;
    const Format *pBestFormat;

    resize = true;
    pDetectionBuffer = NULL;
    currentOffset = inputStream.QueryCurrentOffset();
    detectionBufferSize = DETECTIONBUFFER_MINSIZE;
    bestScore = 0;
    pBestFormat = NULL;

    SkipID3(inputStream);

    while(resize)
    {
        pDetectionBuffer = (byte *)MemRealloc(pDetectionBuffer, detectionBufferSize);
        inputStream.SeekTo(currentOffset);
        nReadBytes = inputStream.ReadBytes(pDetectionBuffer, detectionBufferSize);
        if(detectionBufferSize != nReadBytes)
            break; //end of input reached... we can't do anything anymore
        resize = false;

        for(const UniquePointer<Format>& format : FormatRegistry::Instance().ContainerFormats())
        {
            BufferInputStream detectionBuffer(pDetectionBuffer, detectionBufferSize);

            matchScore = format->Probe(detectionBuffer);

            //check unusual cases
            if(matchScore == Format::FORMAT_MATCH_BUFFER_TOO_SMALL)
            {
                if(detectionBufferSize < DETECTIONBUFFER_MAXSIZE)
                    resize = true;
            }
            else if(matchScore == 1)
            {
                pBestFormat = format.operator->();
                goto end;
            }
            else if(matchScore > bestScore)
            {
                pBestFormat = format.operator->();
                bestScore = matchScore;
            }
        }

        if(resize)
        {
            detectionBufferSize = Math::Min(detectionBufferSize * 2, DETECTIONBUFFER_MAXSIZE);
        }
    }

    end:;
    MemFree(pDetectionBuffer);
    inputStream.SeekTo(currentOffset);

    return pBestFormat;
}

void FormatRegistry::Register(Decoder *decoder, float32 quality)
{
    CodingFormat *codingFormat = const_cast<CodingFormat *>(this->FindCodingFormatById(decoder->GetCodingFormatId()));
    codingFormat->AddDecoder(decoder, quality);
}

void FormatRegistry::Register(Encoder* encoder, float32 quality)
{
    CodingFormat *codingFormat = const_cast<CodingFormat *>(this->FindCodingFormatById(encoder->GetCodingFormatId()));
    codingFormat->AddEncoder(encoder, quality);
}

void FormatRegistry::Release()
{
    this->codingFormats.Release();
    this->formats.Release();
}
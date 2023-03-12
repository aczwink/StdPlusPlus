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
//Audio coding formats
#include "Codecs/MPEG/AAC_CodingFormat.hpp"
#include "Codecs/MPEG/MP3/MP3_CodingFormat.hpp"
#include "Codecs/Xiph/Vorbis_CodingFormat.hpp"
#include "Codecs/AC3_CodingFormat.hpp"
#include "Codecs/PCM/Float32LE/PCM_Float32LE_CodingFormat.hpp"
#include "Codecs/PCM/U8/PCM_U8_CodingFormat.hpp"
#include "Codecs/PCM/S16/PCM_S16BE_CodingFormat.hpp"
#include "Codecs/PCM/S16/PCM_S16_Decoder.hpp"
#include "Codecs/PCM/S8/PCM_S8_CodingFormat.hpp"

//Subtitle coding formats
#include "Codecs/Raw_Subtitles/UTF8PlainText_CodingFormat.hpp"

//Video coding formats
#include "Codecs/Images/PNG_CodingFormat.hpp"
#include "Codecs/MPEG/H264_CodingFormat.hpp"
#include "Codecs/MPEG/MPEG4_Part2_CodingFormat.hpp"
#include "Codecs/MPEG/MS_MPEG4_Part2_V2_CodingFormat.hpp"
#include "Codecs/RawVideo/RawVideo_CodingFormat.hpp"
#include "Codecs/Xiph/Theora_CodingFormat.hpp"

//Container formats
#include "Formats/BMP/BMP_Format.hpp"
#include "Formats/Matroska/MatroskaVideo.hpp"
#include "Formats/Raw/RawContainerImages.hpp"
#include "Formats/WAVE/WAVE_Format.hpp"
#include "Codecs/PCM/S16/PCM_S16LE_CodingFormat.hpp"
#include "Codecs/PCM/S16/PCM_S16_Encoder.hpp"
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

//Private methods
void FormatRegistry::RegisterCodingFormats()
{
    //Audio coding formats
    this->Register(new AAC_CodingFormat);
    this->Register(new AC3_CodingFormat);
    this->Register(new MP3_CodingFormat);
    this->Register(new PCM_Float32LE_CodingFormat);
    this->Register(new PCM_S16BE_CodingFormat);
    this->Register(new PCM_S16LE_CodingFormat);
    this->Register(new PCM_S8_CodingFormat);
    this->Register(new PCM_U8_CodingFormat);
    this->Register(new Vorbis_CodingFormat);


    //Subtitle coding formats
    this->Register(new UTF8PlainText_CodingFormat);


    //Video coding formats
    this->Register(new H264_CodingFormat);
    this->Register(new MPEG4_Part2_CodingFormat);
    this->Register(new MS_MPEG4_Part2_V2_CodingFormat);
    this->Register(new PNG_CodingFormat);
    this->Register(new RawVideo_CodingFormat);
    this->Register(new Theora_CodingFormat);
}

void FormatRegistry::RegisterContainerFormats()
{
    this->Register(new BMP_Format);
    this->Register(new MatroskaVideo);
    this->Register(new PNG_Format);
    this->Register(new WAVE_Format);
}

void FormatRegistry::RegisterDecoders()
{
    this->Register(new PCM_S16_Decoder(true), 0.0f);
    this->Register(new PCM_S16_Decoder(false), 0.0f);
}

void FormatRegistry::RegisterEncoders()
{
    this->Register(new PCM_S16_Encoder(true), 0.0f);
    this->Register(new PCM_S16_Encoder(false), 0.0f);
}
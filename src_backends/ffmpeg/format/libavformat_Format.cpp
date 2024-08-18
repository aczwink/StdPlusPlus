/*
 * Copyright (c) 2020-2024 Amir Czwink (amir130@hotmail.de)
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
#include "libavformat_Format.hpp"
//Local
#include <Std++/Containers/Strings/String.hpp>
#include "libavio_InputStreamWrapper.hpp"
#include "libavformat_Demuxer.hpp"
#include "libavformat_Muxer.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::Multimedia;

//Constructor
libavformat_Format::libavformat_Format(const char *shortName)
{
	this->avInputFormat = av_find_input_format(shortName);
	ASSERT(this->avInputFormat, u8"Wrong short name");

	this->avOutputFormat = av_guess_format(shortName, nullptr, nullptr);
	ASSERT(this->avOutputFormat, u8"Wrong short name");
}

//Public methods
Demuxer *libavformat_Format::CreateDemuxer(SeekableInputStream& seekableInputStream) const
{
	return new libavformat_Demuxer(*this, seekableInputStream);
}

Muxer *libavformat_Format::CreateMuxer(SeekableOutputStream &seekableOutputStream) const
{
	return new libavformat_Muxer(*this, seekableOutputStream);
}

String libavformat_Format::GetExtension() const
{
	return this->avOutputFormat->extensions;
}

void libavformat_Format::GetFormatInfo(FormatInfo &formatInfo) const
{
	formatInfo.supportsByteSeeking = (this->avInputFormat->flags & AVFMT_NO_BYTE_SEEK) == 0;
}

String libavformat_Format::GetName() const
{
	return String(this->avOutputFormat->long_name) + u8" via libavformat (ffmpeg)";
}

DynamicArray<const CodingFormat *> libavformat_Format::GetSupportedCodingFormats(DataType dataType) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return StdXX::DynamicArray<const StdXX::Multimedia::CodingFormat *>();
}

float32 libavformat_Format::Probe(BufferInputStream &buffer) const
{
	static BufferInputStream* lastBuffer = nullptr;
	static const AVInputFormat* lastFormat = nullptr;
	static int lastScore;

	//ugly, but we don't want to probe through all of libavformats formats for every libavformat_Format instance
	if(lastBuffer == &buffer)
	{
		if(lastFormat == this->avInputFormat)
			return lastScore / (float32)AVPROBE_SCORE_MAX;
		return 0;
	}

	AVProbeData avProbeData;
	avProbeData.filename = nullptr;
	avProbeData.buf_size = buffer.QuerySize();
	avProbeData.buf = static_cast<unsigned char *>(av_malloc(avProbeData.buf_size + AVPROBE_PADDING_SIZE));
	avProbeData.mime_type = nullptr;

	buffer.ReadBytes(avProbeData.buf, avProbeData.buf_size);
	MemZero(avProbeData.buf + avProbeData.buf_size, AVPROBE_PADDING_SIZE);

	lastFormat = av_probe_input_format3(&avProbeData, true, &lastScore);
	lastBuffer = &buffer;

	av_free(avProbeData.buf);

	if(lastFormat == this->avInputFormat)
		return lastScore / (float32)AVPROBE_SCORE_MAX;
	return 0;
}
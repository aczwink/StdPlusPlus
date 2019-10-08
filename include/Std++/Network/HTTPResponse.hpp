/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Streams/BufferedOutputStream.hpp>
#include <Std++/Streams/Writers/TextWriter.hpp>
#include <Std++/CommonFileFormats/JSON.hpp>
#include "HTTPHeaders.hpp"

namespace StdXX
{
	class HTTPResponse
	{
	public:
		//Constructor
		inline HTTPResponse(OutputStream& outputStream) : bufferedOutputStream(outputStream), textWriter(bufferedOutputStream, TextCodecType::ASCII),
														  headersWritten(false)
		{
			this->textWriter.SetLineSeparator(u8"\r\n");
		}

		//Destructor
		~HTTPResponse();

		//Methods
		void JSON(const CommonFileFormats::JsonValue& json);
		void WriteHeader(uint16 statusCode);

		//Inline
		inline void SetHeader(const String& header, const String& value)
		{
			ASSERT(!this->headersWritten, u8"Can't set headers when they were already transmitted.");
			this->headers.Set(header, value);
		}

		inline void WriteData(const void* data, uint32 dataSize)
		{
			ASSERT(this->headersWritten, u8"Can't write data before headers have been transmitted.");
			this->bufferedOutputStream.WriteBytes(data, dataSize);
		}

	private:
		//Members
		BufferedOutputStream bufferedOutputStream;
		TextWriter textWriter;
		HTTPHeaders headers;
		bool headersWritten;

		//Methods
		String GetStatusMessage(uint16 statusCode) const;
	};
}
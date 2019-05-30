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
//Class header
#include <Std++/Network/HTTPResponse.hpp>
//Namespaces
using namespace StdXX;

//Destructor
HTTPResponse::~HTTPResponse()
{
	this->bufferedOutputStream.Flush();
}

//Public methods
void HTTPResponse::JSON(const CommonFileFormats::JSONValue& json)
{
	String jsonData = json.Dump().ToUTF8();

	this->SetHeader(u8"Content-Type", u8"application/json");
	this->SetHeader(u8"Content-Length", String::Number(jsonData.GetSize()));
	this->WriteHeader(200);
	this->WriteData(jsonData.GetRawData(), jsonData.GetSize());
}

void HTTPResponse::WriteHeader(uint16 statusCode)
{
	this->textWriter.WriteString(u8"HTTP/1.1 ");
	this->textWriter.WriteString(String::Number(statusCode));
	this->textWriter.WriteString(u8" ");
	this->textWriter.WriteLine(this->GetStatusMessage(statusCode));

	this->bufferedOutputStream << this->headers;
	this->textWriter.WriteLine();

	this->headersWritten = true;
}

//Private methods
String HTTPResponse::GetStatusMessage(uint16 statusCode) const
{
	switch(statusCode)
	{
		case 200:
			return u8"OK";
		case 404:
			return u8"Not Found";
		case 501:
			return u8"Not Implemented";
	}
	NOT_IMPLEMENTED_ERROR;
	return String();
}
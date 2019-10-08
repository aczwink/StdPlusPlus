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
#include <Std++/Network/HTTPServer.hpp>
//Local
#include <Std++/Streams/Writers/StdOut.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/Streams/BufferedInputStream.hpp>
#include <Std++/Time/TimeMisc.hpp>
#include <Std++/Streams/BufferedOutputStream.hpp>
#include <Std++/Streams/Writers/TextWriter.hpp>
//Namespaces
using namespace StdXX;

//Public methods
void HTTPServer::Serve()
{
	while(!this->shutdown)
	{
		UniquePointer<TCPSocket> client = this->socket.WaitForIncomingConnections(1000);
		if(client == nullptr)
			continue; //time out
		BufferedInputStream bufferedInputStream(client->GetInputStream()); //important!
		TextReader textReader(bufferedInputStream, TextCodecType::ASCII);

		//wait for data
		if(client->GetInputStream().GetBytesAvailable() == 0)
		{
			if(!client->WaitForData(10000)) //wait max 10 secs for data to arrive
				continue;
		}

		//request line
		String reqLine = textReader.ReadLine();
		DynamicArray<String> reqParts = reqLine.Split(u8" ");

		ASSERT(reqParts.GetNumberOfElements() == 3, u8"REPORT THIS PLEASE!");
		ASSERT(reqParts[2] == u8"HTTP/1.1", u8"REPORT THIS PLEASE!");
		ASSERT(reqParts[0] == u8"GET", u8"REPORT THIS PLEASE!");
		Path path = reqParts[1];

		//parse headers
		HTTPHeaders headers;
		while(true)
		{
			String line = textReader.ReadLine();
			//end of headers
			if(line.IsEmpty())
				break;

			//find :
			uint32 colonPos = line.Find(u8":");
			ASSERT(colonPos != Unsigned<uint32>::Max(), u8"REPORT THIS PLEASE!");

			headers.Set(line.SubString(0, colonPos), line.SubString(colonPos+1).Trim());
		}

		HTTPResponse response(client->GetOutputStream());
		this->OnGETRequest(path, headers, response);
	}
}

//Event handlers
void HTTPServer::OnGETRequest(const Path& requestPath, const HTTPHeaders& requestHeaders, HTTPResponse& response)
{
	response.WriteHeader(501);
}

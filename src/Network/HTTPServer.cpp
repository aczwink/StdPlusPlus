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
#include <Std++/Streams/StdOut.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/Streams/BufferedInputStream.hpp>
//Namespaces
using namespace StdXX;

//Public methods
void HTTPServer::ServeForever()
{
	while(true)
	{
		UniquePointer<TCPSocket> client = this->socket.WaitForIncomingConnections(Unsigned<uint64>::Max());
		BufferedInputStream bufferedInputStream(client->GetInputStream()); //important!
		TextReader textReader(bufferedInputStream, TextCodecType::ASCII);

		//print header
		while(true)
		{
			String line = textReader.ReadLine();
			stdOut << line << endl;

			//end of headers
			if(line.IsEmpty())
				break;
		}
	}
}
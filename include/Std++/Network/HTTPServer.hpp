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
#include <Std++/Filesystem/Path.hpp>
#include "HTTPHeaders.hpp"
#include "HTTPResponse.hpp"
#include "TCPServerSocket.hpp"

namespace StdXX
{
	class HTTPServer
	{
	public:
		//Constructor
		inline HTTPServer(const NetAddress& netAddress, uint16 port) : socket(netAddress, port), shutdown(false)
		{
		}

		//Methods
		/**
		 * Serves HTTP requests until Shutdown is called.
		 * The function checks each second whether it should shutdown.
		 */
		void Serve();

		//Inline
		inline uint16 GetBoundPort() const
		{
			return this->socket.GetBoundPort();
		}

		inline void Shutdown()
		{
			this->shutdown = true;
		}

	protected:
		//Event handlers
		virtual void OnGETRequest(const Path& requestPath, const HTTPHeaders& requestHeaders, HTTPResponse& response);

	private:
		//Members
		bool shutdown;
		TCPServerSocket socket;
	};
}
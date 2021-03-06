/*
* Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/ExtensionManager.hpp>
//Extensions
#include "ffmpeg/libavcodec_Extension.hpp"
#ifdef _STDXX_EXTENSION_LIBAVFORMAT
#include "ffmpeg/libavformat_Extension.hpp"
#endif
#ifdef _STDXX_EXTENSION_OPENSSL
#include "OpenSSL/OpenSSL_Extension.hpp"
#endif
//Namespaces
using namespace _stdxx_;

//Global functions
void RegisterExtensions()
{
#define ADD_EXTENSION(extension) ExtensionManager::GetRootInstance().RegisterExtension(extension);
#ifdef _STDXX_EXTENSION_LIBAVCODEC
	libavcodec_Extension *libavcodec_backend = new libavcodec_Extension;
	ADD_EXTENSION(libavcodec_backend);
#endif
#ifdef _STDXX_EXTENSION_LIBAVFORMAT
	libavformat_Extension *libavformat_backend = new libavformat_Extension;
	ADD_EXTENSION(libavformat_backend);
#endif
#ifdef _STDXX_EXTENSION_OPENSSL
	OpenSSL_Extension* openSSLExtension = new OpenSSL_Extension;
	ADD_EXTENSION(openSSLExtension);
#endif
#undef ADD_BACKEND
}
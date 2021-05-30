/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
#include "libavformat_Extension.hpp"
//Local
#include "format/libavformat_Format.hpp"
//Namespaces
using namespace _stdxx_;

//Public methods
void libavformat_Extension::Load()
{
	ffmpeg_Extension::Load();

	this->LoadCodingFormatIdMap();
	this->LoadPatentedCodingFormatIdMap();

	StdXX::Multimedia::Format::Register(new libavformat_Format("avi"));
	StdXX::Multimedia::Format::Register(new libavformat_Format("mp4"));
}

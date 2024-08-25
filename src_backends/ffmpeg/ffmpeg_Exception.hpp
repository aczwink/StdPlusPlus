/*
* Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
//Global
extern "C"
{
#include <libavutil/avutil.h>
}
//Local
#include <Std++/Errorhandling/Exception.hpp>


class ffmpeg_Exception : public StdXX::Exception
{
public:
	//Constructor
	inline ffmpeg_Exception(int errorCode)
	{
		char buffer[4096];
		av_strerror(errorCode, buffer, sizeof(buffer));
		this->errorMessage = StdXX::String::CopyRawString(buffer);
	}

	//Inline
	inline StdXX::String Description() const
	{
		return u8"An exception in libavcodec/libavformat extensions occured: " + this->errorMessage;
	}

private:
	//State
	StdXX::String errorMessage;
};
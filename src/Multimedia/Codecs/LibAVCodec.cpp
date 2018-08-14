/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#ifdef _AC_LIB_USEAVCODEC
#include "LibAVCodec.hpp"
//Local
#include <Std++/Containers/Map/Map.hpp>
#include <Std++/Multimedia/Pixmaps/YCbCr/YCbCr420Image.hpp>
#include <Std++/Multimedia/AudioBuffer.hpp>
#include <Std++/Multimedia/AudioFrame.hpp>
#include <Std++/Multimedia/VideoFrame.hpp>
#include <Std++/Multimedia/VideoStream.hpp>

//Local functions
static ChannelLayout MapChannels(int nChannels)
{
	switch(nChannels)
	{
		case 1:
			return ChannelLayout::Mono;
		case 2:
			return ChannelLayout::Stereo;
	}

	NOT_IMPLEMENTED_ERROR;
}
#endif
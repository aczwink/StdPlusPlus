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
//Class header
#include "ExternalVideoCodec.hpp"
//Local
#include "LibAV_VideoDecoder.hpp"
#include "../../../src_backends/libavcodec/Multimedia/libavcodec_Encoder.hpp"

//Public methods
Decoder *ExternalVideoCodec::CreateDecoder(Stream &stream) const
{
#ifdef _AC_LIB_USEAVCODEC
	return new LibAV_VideoDecoder(stream, this->GetId());
#else
	return nullptr;
#endif
}

Encoder *ExternalVideoCodec::CreateEncoder(Stream &stream) const
{
#ifdef _STDXX_BACKEND_LIBAVCODEC
	return new _stdxx_::libavcodec_Encoder(stream, this->GetId());
#endif
	return nullptr;
}

AParser *ExternalVideoCodec::CreateParser() const
{
	return nullptr;
}
/*
 * Copyright (c) 2023-2024 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/Multimedia/DecoderContext.hpp>
#include <Std++/Multimedia/Frame.hpp>

class RawSinglePlaneVideoDecoderContext : public StdXX::Multimedia::DecoderContext
{
public:
	//Constructor
	inline RawSinglePlaneVideoDecoderContext(DecodingParameters& decodingParameters) : DecoderContext(decodingParameters)
	{
	}

	void Decode(const IPacket &packet) override
	{
		const auto& fmt = *this->Parameters().video.pixelFormat;
		const auto& size = this->Parameters().video.size;

		ASSERT_EQUALS(1, fmt.nPlanes);

		uint32 imageSize = fmt.ComputeLineSize(0, size.width) * size.height;
		Pixmap* pixmap = new Pixmap(size, fmt);

		MemCopy(pixmap->GetPlane(0), packet.GetData(), imageSize);

		this->AddFrame(new Frame(pixmap));
	}
};
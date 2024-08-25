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
#include <Std++/Math/Vector4/Vector4.inl>
#include <Std++/Multimedia/Pixmap.hpp>
#include <Std++/Multimedia/PixelFormat.hpp>

/*
Sources:
https://wiki.multimedia.cx/index.php?title=S3TC
https://github.com/svn2github/worldwind/tree/master/WWAndroid/src/gov/nasa/worldwind/util/dds
 */

class S3TC_DXT1_DecoderContext : public StdXX::Multimedia::DecoderContext
{
public:
	//Constructor
	inline S3TC_DXT1_DecoderContext(DecodingParameters& decodingParameters) : DecoderContext(decodingParameters)
	{
		this->Parameters().video.pixelFormat = PixelFormat(NamedPixelFormat::BGR_24);
		this->blockSize = this->Parameters().video.pixelFormat->ComputeBlockSize(0);
	}

	void Decode(const IPacket &packet) override
	{
		uint8 i;
		uint16 c0, c1, x, y;
		uint32 pixels;
		Math::Vector4<uint32> c[4];

		BufferInputStream inputStream(packet.GetData(), packet.GetSize());
		DataReader input(false, inputStream);

		Pixmap* pixmap = new Pixmap(this->Parameters().video.size, *this->Parameters().video.pixelFormat);

		for(y = 0; y < this->Parameters().video.size.height; y += 4)
		{
			for(x = 0; x < this->Parameters().video.size.width; x += 4)
			{
				c0 = input.ReadUInt16();
				c1 = input.ReadUInt16();
				this->UnpackRGB565(c0, c[0]);
				this->UnpackRGB565(c1, c[1]);

				if(c0 > c1)
				{
					c[2] = (2_u32 * c[0] + c[1]) / 3;
					c[3] = (2_u32 * c[1] + c[0]) / 3;
				}
				else
				{
					c[2] = (c[0] + c[1]) / 2;
					c[3] = {}; //set to zero
				}
				pixels = input.ReadUInt32();

				for(i = 0; i < 16; i++)
				{
					this->SetPixel(*pixmap, x + (i % 4), this->Parameters().video.size.height - 1 - (y + (i / 4)), c[pixels & 3].e);
					pixels >>= 2;
				}
			}
		}

		this->AddFrame(new Frame(pixmap));
	}

private:
	//State
	uint8 blockSize;

	//Methods
	void UnpackRGB565(uint16 packed, Math::Vector4<uint32> &refUnpacked)
	{
		refUnpacked.x = packed >> 11;
		refUnpacked.y = (packed >> 5) & 0x3F;
		refUnpacked.z = packed & 0x1F;

		refUnpacked.x *= (256 / 32);
		refUnpacked.y *= (256 / 64);
		refUnpacked.z *= (256 / 32);
	}

	void SetPixel(Pixmap& pixmap, uint16 x, uint16 line, const uint32 rgb[3])
	{
		const auto& fmt = *this->Parameters().video.pixelFormat;

		for(uint8 i = 0; i < 3; i++)
		{
			const auto& cc = fmt.colorComponents[i];

			auto* data = static_cast<uint8 *>(pixmap.GetPlane(cc.planeIndex));
			data += line * pixmap.GetLineSize(cc.planeIndex) + x * this->blockSize + (cc.shift / 8);

			*data = static_cast<uint8>(rgb[i]);
		}
	}
};
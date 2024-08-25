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
//Class header
#include <Std++/Multimedia/RGBPixmapView.hpp>
//Local
#include <Std++/Unsigned.hpp>
//Namespaces
using namespace StdXX::Multimedia;

//Public methods
void RGBPixmapView::SetAllPixels(const Math::Vector3S &rgb)
{
	for(uint16 line = 0; line < this->rgbPixmap.GetSize().height; line++)
	{
		for(uint16 x = 0; x < this->rgbPixmap.GetSize().width; x++)
			this->SetPixel(x, line, rgb);
	}
}

void RGBPixmapView::SetPixel(uint16 x, uint16 line, const Math::Vector3S& rgb)
{
	const auto& fmt = this->rgbPixmap.GetPixelFormat();

	for(uint8 i = 0; i < 3; i++)
	{
		const auto& cc = fmt.colorComponents[i];

		auto* data = static_cast<uint8 *>(this->rgbPixmap.GetPlane(cc.planeIndex));
		data += line * this->rgbPixmap.GetLineSize(cc.planeIndex) + x * fmt.ComputeBlockSize(0) + (cc.shift / 8);

		if(cc.isFloat)
		{
			ASSERT_EQUALS(32, cc.nBits);
			*(float32*)data = rgb[i];
		}
		else
		{
			switch(cc.nBits)
			{
				case 8:
					*data = (uint8)(rgb[i] * Unsigned<uint8>::Max());
					break;
				default:
					NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
		}
	}
}

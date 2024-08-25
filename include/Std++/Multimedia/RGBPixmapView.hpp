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
#pragma once
//Local
#include <Std++/Math/Vector3.hpp>
#include "Pixmap.hpp"

namespace StdXX::Multimedia
{
	class RGBPixmapView
	{
	public:
		//Constructor
		inline RGBPixmapView(Pixmap& rgbPixmap) : rgbPixmap(rgbPixmap)
		{
		}

		//Methods
		void SetAllPixels(const Math::Vector3S& rgb);
		void SetPixel(uint16 x, uint16 line, const Math::Vector3S& rgb);

		//Inline
		inline void SetAllPixels(float32 r, float32 g, float32 b)
		{
			this->SetAllPixels({ r, g, b});
		}

		inline void SetPixel(uint16 x, uint16 line, float32 r, float32 g, float32 b)
		{
			this->SetPixel(x, line, { r, g, b});
		}

	private:
		//State
		Pixmap& rgbPixmap;
	};
}
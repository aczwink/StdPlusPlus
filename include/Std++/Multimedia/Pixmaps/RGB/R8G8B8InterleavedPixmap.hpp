/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/Pixmaps/RGBPixmap.hpp>
#include <Std++/Containers/Table/FixedTable.hpp>

namespace StdXX
{
	namespace Multimedia
	{
		class STDPLUSPLUS_API R8G8B8InterleavedPixmap : public RGBPixmap
		{
		public:
			//Constructor
			inline R8G8B8InterleavedPixmap(uint16 width, uint16 height) : RGBPixmap(width, height),
																		  pixels(height, width)
			{
			}

		private:
			//Members
			FixedTable<Math::Vector3<uint8>> pixels;
		};
	}
}
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
#pragma once

/*
Mircosoft FourCCs:
http://www.faqs.org/rfcs/rfc2361.html
https://msdn.microsoft.com/de-de/library/windows/desktop/bb970509(v=vs.85).aspx
*/
//FourCCs are always little endian
#define FOURCC(fcc) (fcc[0] | (fcc[1] << 8) | (fcc[2] << 16) | (fcc[3] << 24))

namespace StdXX
{
	namespace Multimedia
	{
		/*
		 * In order to not break binary-compatibility, assign each enumerator a unique value!
		 */
		enum class CodingFormatId
		{
			Unknown = 0,

			//Video codecs
			MS_MPEG4Part2V2 = FOURCC(u8"MP42")
		};
	}
}
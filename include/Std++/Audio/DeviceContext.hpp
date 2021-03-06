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
#include <Std++/Math/Vector3.hpp>

namespace StdXX
{
	namespace Audio
	{
		//Forward declarations
		class Buffer;
		class Source;

		class DeviceContext
		{
		public:
			//Destructor
			virtual ~DeviceContext(){}

			//Abstract
			virtual Buffer *CreateBuffer() = 0;
			virtual Source *CreateSource() = 0;
			virtual void SetListenerOrientation(const Math::Vector3S &at, const Math::Vector3S &up) = 0;
			virtual void SetListenerPosition(const Math::Vector3S &pos) = 0;
			virtual void SetListenerVelocity(const Math::Vector3S &vel) = 0;
		};
	}
}
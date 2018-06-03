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

namespace StdPlusPlus
{
	namespace Audio
	{
		//Forward declarations
		class Buffer;

		class Source
		{
		public:
			//Destructor
			virtual ~Source(){}

			//Abstract
			virtual bool IsPlaying() const = 0;
			virtual void Play() = 0;
			virtual void SetBuffer(const Buffer *buffer) = 0;
			virtual void SetGain(float32 gain) = 0;
			virtual void SetLooping(bool loop) = 0;
			virtual void SetPitch(float32 pitch) = 0;
			virtual void SetPosition(const Math::Vector3s &pos) = 0;
			virtual void SetVelocity(const Math::Vector3s &vel) = 0;
		};
	}
}
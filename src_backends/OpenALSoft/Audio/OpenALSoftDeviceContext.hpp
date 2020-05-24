/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
//Global
#ifdef XPC_OS_DARWIN
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
//Local
#include <Std++/Audio/DeviceContext.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Audio;

class OpenALSoftDeviceContext : public DeviceContext
{
public:
	//Constructor
	OpenALSoftDeviceContext(ALCdevice *device);

	//Destructor
	~OpenALSoftDeviceContext();

	//Methods
	Buffer *CreateBuffer() override;
	Source *CreateSource() override;
	void SetListenerOrientation(const Math::Vector3S &at, const Math::Vector3S &up) override;
	void SetListenerPosition(const Math::Vector3S &pos) override;
	void SetListenerVelocity(const Math::Vector3S &vel) override;

	//Inline
	inline void Bind()
	{
		if(currentContext != this->context)
		{
			ALCboolean result = alcMakeContextCurrent(this->context);
			ASSERT_EQUALS(ALC_TRUE, result);
			currentContext = this->context;
		}
	}

private:
	//Members
	ALCcontext *context;

	//Class members
	static ALCcontext *currentContext;
};
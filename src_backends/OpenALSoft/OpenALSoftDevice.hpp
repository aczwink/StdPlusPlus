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
//Global
#ifdef XPC_OS_DARWIN
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
//Local
#include <Std++/Devices/AudioDevice.hpp>
#include "Audio/OpenALSoftDeviceContext.hpp"
//Namespaces
using namespace StdXX;

class OpenALSoftDevice : public AudioDevice
{
public:
	//Constructor
	inline OpenALSoftDevice(const ALchar *deviceName)
	{
		this->device = alcOpenDevice(deviceName);
	}

	//Destructor
	~OpenALSoftDevice()
	{
		alcCloseDevice(this->device);
	}

	//Methods
	Audio::DeviceContext *CreateDeviceContext() const override
	{
		return new OpenALSoftDeviceContext(this->device);
	}

	String GetName() const override
	{
		return String::CopyRawString(alcGetString(this->device, ALC_DEVICE_SPECIFIER));
	}

private:
	//Members
	ALCdevice *device;
};
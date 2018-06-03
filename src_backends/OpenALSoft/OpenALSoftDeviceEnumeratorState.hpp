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
#include <AL/al.h>
//Local
#include <Std++/_Backends/DeviceEnumeratorState.hpp>
#include <Std++/Containers/Strings/StringUtil.h>
#include "OpenALSoftDevice.hpp"
//Namespaces
using namespace StdPlusPlus;

class OpenALSoftDeviceEnumeratorState : public _stdpp::DeviceEnumeratorState
{
public:
	//Constructor
	inline OpenALSoftDeviceEnumeratorState()
	{
		//get devices list
		//ALCdevice *device = alcOpenDevice(NULL);
		//ALboolean enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
		//this->devicesListString = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

		this->devicesListString = u8"OpenAL Soft\0\0";
		this->current = this->devicesListString;
	}

	//Methods
	AutoPointer<Device> GetNextDevice() override
	{
		if(this->current == nullptr | *this->current == 0)
			return nullptr;
		const ALchar *currentDeviceName = this->current;
		this->current += GetStringLength(currentDeviceName) + 1;
		return new OpenALSoftDevice(currentDeviceName);
	}

private:
	//Members
	const ALchar *devicesListString;
	const ALchar *current;
};
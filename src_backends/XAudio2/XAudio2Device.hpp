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
#include <initguid.h>
#include <mmdeviceapi.h>
#include <XAudio2.h>
//Local
#include <Std++/Devices/AudioDevice.hpp>
//Namespaces
using namespace StdXX;

class XAudio2Device : public AudioDevice
{
public:
	//Constructor
	inline XAudio2Device(IXAudio2* pXAudio2, IMMDevice* device) : pXAudio2(pXAudio2), device(device)
	{
	}

	//Destructor
	~XAudio2Device();

	//Methods
	Audio::DeviceContext * CreateDeviceContext() const override;
	String GetName() const override;

private:
	//Members
	IXAudio2* pXAudio2;
	IMMDevice* device;
};
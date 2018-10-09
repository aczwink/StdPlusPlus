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
#include <mmdeviceapi.h>
//Local
#include <Std++/_Backends/DeviceEnumeratorState.hpp>
#include "XAudio2Device.hpp"
//Namespaces
using namespace StdXX;

class XAudio2DeviceEnumeratorState : public _stdxx_::DeviceEnumeratorState
{
public:
	//Constructor
	inline XAudio2DeviceEnumeratorState(IXAudio2* pXAudio2) : pXAudio2(pXAudio2)
	{
		IMMDeviceEnumerator *enumerator;
		HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void **)&enumerator);
		ASSERT(hr == S_OK, u8"Report this please!");
		
		hr = enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &this->devCollection);
		ASSERT(hr == S_OK, u8"Report this please!");
		hr = this->devCollection->GetCount(&this->nDevices);
		ASSERT(hr == S_OK, u8"Report this please!");
		this->nextDevIdx = 0;
		
		enumerator->Release();
	}

	//Destructor
	~XAudio2DeviceEnumeratorState()
	{
		this->devCollection->Release();
	}

	//Methods
	AutoPointer<Device> GetNextDevice() override
	{
		if (this->nextDevIdx >= this->nDevices)
			return nullptr;

		IMMDevice* device;
		HRESULT hr = this->devCollection->Item(this->nextDevIdx++, &device);
		ASSERT(hr == S_OK, u8"Report this please!");

		return new XAudio2Device(pXAudio2, device);
	}

private:
	//Members
	IXAudio2* pXAudio2;
	IMMDeviceCollection* devCollection;
	UINT nextDevIdx;
	UINT nDevices;
};
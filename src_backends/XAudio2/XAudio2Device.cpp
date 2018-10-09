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
//Class header
#include "XAudio2Device.hpp"
//Local
#include "Audio/XAudio2DeviceContext.hpp"

//Destructor
XAudio2Device::~XAudio2Device()
{
	this->device->Release();
}

//Public methods
Audio::DeviceContext * XAudio2Device::CreateDeviceContext() const
{
	LPWSTR deviceId;
	HRESULT hr = this->device->GetId(&deviceId);
	ASSERT(hr == S_OK, u8"Report this please!");

	/*
	IPropertyStore* propertyStore;
	HRESULT hr = this->device->OpenPropertyStore(STGM_READ, &propertyStore);
	ASSERT(hr == S_OK, u8"Report this please!");
	
	PROPVARIANT value;
	hr = propertyStore->GetValue(PKEY_AudioEndpoint_GUID, &value);
	ASSERT(hr == S_OK, u8"Report this please!");
	*/

	XAudio2DeviceContext* dc = new XAudio2DeviceContext(this->pXAudio2, deviceId); //TODO: WHICH FUCKING ID SHOULD BE PASSED TO CreateMasteringVoice??????!!!!

	//propertyStore->Release();
	CoTaskMemFree(deviceId);
	
	return dc;
}

String XAudio2Device::GetName() const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return String();
}
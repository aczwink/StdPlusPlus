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
#include "XAudio2DeviceContext.hpp"
//Local
#include "XAudio2Buffer.hpp"
#include "XAudio2Source.hpp"

//Constructor
XAudio2DeviceContext::XAudio2DeviceContext(IXAudio2* pXAudio2, LPWSTR deviceId) : pXAudio2(pXAudio2)
{
	this->pXAudio2MasteringVoice = nullptr;
	//TODO: deviceId can't be passed currently because the stupid documentation does not mention WHICH kind of id should be passed here. I tried the device id, its direct sound property shit bla id... nothing works -.-
	HRESULT hr = this->pXAudio2->CreateMasteringVoice(&this->pXAudio2MasteringVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, nullptr, nullptr);
	ASSERT(hr == S_OK, u8"Report this please!");
}

//Destructor
XAudio2DeviceContext::~XAudio2DeviceContext()
{
	this->pXAudio2MasteringVoice->DestroyVoice();
}

//Public methods
Buffer * XAudio2DeviceContext::CreateBuffer()
{
	return new XAudio2Buffer();
}

Source * XAudio2DeviceContext::CreateSource()
{
	return new XAudio2Source(this->pXAudio2);
}

void XAudio2DeviceContext::SetListenerOrientation(const Math::Vector3s & at, const Math::Vector3s & up)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void XAudio2DeviceContext::SetListenerPosition(const Math::Vector3s & pos)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

void XAudio2DeviceContext::SetListenerVelocity(const Math::Vector3s & vel)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}
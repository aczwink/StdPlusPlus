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
//Global
#include <XAudio2.h>
//Local
#include <Std++/Audio/DeviceContext.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Audio;

class XAudio2DeviceContext : public DeviceContext
{
public:
	//Constructor
	XAudio2DeviceContext(IXAudio2* pXAudio2, LPWSTR deviceId);

	//Destructor
	~XAudio2DeviceContext();

	//Methods
	Buffer * CreateBuffer() override;
	Source * CreateSource() override;
	void SetListenerOrientation(const Math::Vector3s & at, const Math::Vector3s & up) override;
	void SetListenerPosition(const Math::Vector3s & pos) override;
	void SetListenerVelocity(const Math::Vector3s & vel) override;

private:
	//Members
	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pXAudio2MasteringVoice;
};
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
#else
#include <AL/al.h>
#endif
//Local
#include <Std++/Audio/Source.hpp>
#include "OpenALSoftDeviceContext.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::Audio;

class OpenALSoftSource : public Source
{
public:
	//Constructor
	OpenALSoftSource(OpenALSoftDeviceContext &deviceContext);

	//Destructor
	~OpenALSoftSource();

	//Methods
	bool IsPlaying() const override;
	void Play() override;
	void SetBuffer(const Buffer *buffer) override;
	void SetGain(float32 gain) override;
	void SetLooping(bool loop) override;
	void SetPitch(float32 pitch) override;
	void SetPosition(const Math::Vector3s &pos) override;
	void SetVelocity(const Math::Vector3s &vel) override;

private:
	//Members
	OpenALSoftDeviceContext &deviceContext;
	ALuint id;
};
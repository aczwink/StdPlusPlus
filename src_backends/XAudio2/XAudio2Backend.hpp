/*
 * Copyright (c) 2018-2019 Amir Czwink (amir130@hotmail.de)
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
#ifdef _STDXX_BACKEND_XAUDIO2
//Global
#include <XAudio2.h>
//Local
#include <Std++/_Backends/AudioDevice.hpp>
#include "XAudio2DeviceEnumeratorState.hpp"

namespace StdXX
{
	class XAudio2Backend : public AudioBackend
	{
	public:
		//Methods
		UniquePointer<_stdxx_::DeviceEnumeratorState> GetDeviceEnumeratorState() const override
		{
			return new XAudio2DeviceEnumeratorState(this->pXAudio2);
		}
		
		void Load() override
		{
			CoInitialize(nullptr);

#if WINVER < _WIN32_WINNT_WIN8
			//this unfortunately is needed on windows 7-.-
			//reason:https://blogs.msdn.microsoft.com/chuckw/2015/10/09/known-issues-xaudio-2-7
#ifdef XPC_BUILDTYPE_DEBUG
			this->hXAudioModule = LoadLibraryA("XAudioD2_7.DLL");
#else
			this->hXAudioModule = LoadLibraryA("XAudio2_7.DLL");
#endif
			ASSERT(this->hXAudioModule, u8"Report this please!");
#else
			this->hXAudioModule = nullptr;
#endif
			DWORD flags = 0;
#ifdef XPC_BUILDTYPE_DEBUG
			//flags |= XAUDIO2_DEBUG_ENGINE;
#endif
			this->pXAudio2 = nullptr;
			HRESULT hr = XAudio2Create(&this->pXAudio2, flags, XAUDIO2_DEFAULT_PROCESSOR);
			ASSERT(hr == S_OK, u8"Report this please!");
		}
		
		void Unload() const override
		{
			this->pXAudio2->Release();

			if (this->hXAudioModule)
				FreeLibrary(this->hXAudioModule);

			CoUninitialize();
		}

	private:
		//Members
		HMODULE hXAudioModule;
		IXAudio2* pXAudio2;
	};
}
#endif
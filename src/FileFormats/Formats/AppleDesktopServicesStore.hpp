/*
 * Copyright (c) 2022-2025 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/FileFormats/CommonFileFormats.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>

//https://en.wikipedia.org/wiki/.DS_Store
//TODO: file name is always .DS_Store
class AppleDesktopServicesStore : public FileFormat
{
public:
	//Public methods
	FixedArray<String> GetExtensions() const override
	{
		return {{u8"DS_Store"}};
	}

	FixedArray<String> GetMediaTypes() const override
	{
		return {{FileMetaData::appleDesktopServicesStore}};
	}

	String GetName() const override
	{
		return u8"Apple Desktop Services Store";
	}

	float32 Probe(SeekableInputStream& seekableInputStream) const override
	{
		uint8 sig[8];

		uint32 nBytesRead = seekableInputStream.ReadBytes(sig, sizeof(sig));
		if(nBytesRead == sizeof(sig))
		{
			const static uint8 expectedSig[8] = {0x00, 0x00, 0x00, 0x01, 0x42, 0x75, 0x64, 0x31};

			return MemCmp(sig, expectedSig, sizeof(sig)) == 0;
		}

		return 0;
	}
};
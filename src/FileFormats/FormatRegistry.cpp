/*
 * Copyright (c) 2025 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileFormats/FormatRegistry.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileFormats;
//Formats
#include "Formats/AppleDesktopServicesStore.hpp"
#include "Formats/AppleDoubleFormat.hpp"

//Constructor
FormatRegistry::FormatRegistry()
{
	ShutdownManager::Instance().Register(this);

	this->RegisterFormats();
}

//Public methods
const FileFormat *FormatRegistry::ProbeFormat(SeekableInputStream& inputStream) const
{
	uint64 startOffset = inputStream.QueryCurrentOffset();

	const FileFormat* bestFormat = nullptr;
	float32 bestScore = 0;
	for(const auto& format : this->formats)
	{
		inputStream.SeekTo(startOffset);

		float32 score = format->Probe(inputStream);
		if(score > bestScore)
		{
			bestFormat = format.operator->();
			bestScore = score;
		}
	}

	inputStream.SeekTo(startOffset);
	return bestFormat;
}

void FormatRegistry::Release()
{
	this->formats.Release();
}

//Private methods
void FormatRegistry::RegisterFormats()
{
	this->formats.Push(new AppleDoubleFormat);
	this->formats.Push(new AppleDesktopServicesStore);
}

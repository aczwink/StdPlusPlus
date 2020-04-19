/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/FileSystem/WritableFileSystem.hpp>
//Local
#include <Std++/FileSystem/Format.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
void WritableFileSystem::CreateDirectoryTree(const Path &directoryPath)
{
	Path p = directoryPath.IsAbsolute() ? directoryPath.GetString().SubString(1) : directoryPath;
	this->GetDirectory(String(u8"/"))->CreateDirectoryTree(p);
}

//Class functions
UniquePointer<WritableFileSystem> WritableFileSystem::Create(const String &id, const Path &path)
{
	const Format *const& fileSystemFormat = Format::GetFormatById(id);
	if(fileSystemFormat)
		return fileSystemFormat->CreateFileSystem(path);
	return nullptr;
}

UniquePointer<WritableFileSystem> WritableFileSystem::LoadFromFile(const Path &p)
{
	const Format *bestFormat = Format::FindBestFormat(p);
	if(bestFormat)
		return bestFormat->OpenFileSystem(p, true);
	return nullptr;
}
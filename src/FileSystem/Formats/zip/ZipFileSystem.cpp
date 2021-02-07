/*
* Copyright (c) 2019-2021 Amir Czwink (amir130@hotmail.de)
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
/*//Class header
#include "ZipFileSystem.hpp"
//Local
#include <Std++/Streams/BufferedInputStream.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/Streams/Writers/StdOut.hpp>
#include <Std++/Streams/FileOutputStream.hpp>
#include "ZipFile.hpp"
#include "CentralDirectoryRecord.hpp"
#include "ZipLink.hpp"
//Namespaces
using namespace _stdxx_;
using namespace StdXX;
using namespace StdXX::FileSystem;

//Constructors
ZipFileSystem::ZipFileSystem(const Path &path)
	: root(new ZipDirectory(*this)), isFlushed(false)
{
	{
		//create file and fail if it exists
		FileOutputStream fileCreator(path);
	}
	this->writableStream = new FileUpdateStream(path);
}

ZipFileSystem::ZipFileSystem(const Path &path, const OpenOptions& openOptions, uint64 endOfCentralDirectoryOffset, bool writable)
	: root(new ZipDirectory(*this)), isFlushed(true)
{
	if(openOptions.password.HasValue())
		this->password = *openOptions.password;

	if(writable)
		this->writableStream = new FileUpdateStream(path);
	else
		this->readOnlyInputStream = new FileInputStream(path);
	EndOfCentralDirectory endOfCentralDirectory = this->ReadEndOfCentralDirectory(endOfCentralDirectoryOffset);
	this->ReadCentralDirectory(endOfCentralDirectory);
}

//Public methods
void ZipFileSystem::CreateLink(const Path &linkPath, const Path &linkTargetPath)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

bool ZipFileSystem::Exists(const Path &path) const
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
	return false;
}

void ZipFileSystem::Flush()
{
	if(this->isFlushed)
		return;

	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

AutoPointer<Directory> ZipFileSystem::GetRoot()
{
	return this->root;
}

AutoPointer<const Directory> ZipFileSystem::GetRoot() const
{
	return this->root;
}

void ZipFileSystem::Move(const Path &from, const Path &to)
{
	NOT_IMPLEMENTED_ERROR; //TODO: implement me
}

SpaceInfo ZipFileSystem::QuerySpace() const
{
	return {
		.freeSize = 0,
		.totalSize = this->readOnlyInputStream->QuerySize(),
	};
}

//Private methods
void ZipFileSystem::ReadCentralDirectory(const EndOfCentralDirectory& record)
{
	this->InputStream().SeekTo(record.centralDirectoryOffset);
	BufferedInputStream bufferedInputStream(this->InputStream());

	for(uint16 i = 0; i < record.nEntries; i++)
	{
	    CentralDirectoryRecord centralDirectoryRecord(bufferedInputStream);

        String nodeName;
        AutoPointer<Directory> dir;
        Node* node;
        if(centralDirectoryRecord.DetermineType() == NodeType::Directory)
        {
	        Path directoryPath = centralDirectoryRecord.path.GetParent();
	        nodeName = directoryPath.GetName();
	        dir = this->GetDirectory(directoryPath.GetParent());
	        node = new ZipDirectory(*this);
        }
        else
        {
	        nodeName = centralDirectoryRecord.path.GetName();
	        dir = this->GetDirectory(centralDirectoryRecord.path.GetParent());

	        uint64 currentOffset = this->InputStream().GetCurrentOffset();
			this->InputStream().SeekTo(centralDirectoryRecord.localFileHeaderOffset);
	        switch(centralDirectoryRecord.DetermineType())
	        {
		        case NodeType::File:
			        node = new ZipFile(centralDirectoryRecord, *this);
			        break;
		        case NodeType::Link:
		        	node = new ZipLink(centralDirectoryRecord, *this);
		        	break;
	        }
			this->InputStream().SeekTo(currentOffset);
        }

		AutoPointer<ZipDirectory> zipDir = dir.Cast<ZipDirectory>();
		zipDir->AddChild(nodeName, node);
	}
}

EndOfCentralDirectory ZipFileSystem::ReadEndOfCentralDirectory(uint64 offset)
{
	this->InputStream().SeekTo(offset);
	BufferedInputStream bufferedInputStream(this->InputStream());
	DataReader dataReader(false, bufferedInputStream);

	EndOfCentralDirectory endOfCentralDirectory;

	bufferedInputStream.Skip(8);
	bufferedInputStream.Skip(2);
	endOfCentralDirectory.nEntries = dataReader.ReadUInt16();
	bufferedInputStream.Skip(4);
	endOfCentralDirectory.centralDirectoryOffset = dataReader.ReadUInt32();

	return endOfCentralDirectory;
}*/
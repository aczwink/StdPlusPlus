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
//Local
#include <Std++/Compression/CompressionAlgorithm.hpp>
#include <Std++/Containers/Array/FixedArray.hpp>
#include <Std++/Filesystem/ContainerFileSystem.hpp>
#include <Std++/Optional.hpp>

namespace _stdxx_
{
	enum class PropertyId : byte
	{
		kEnd = 0x00,
		kHeader = 0x01,
		kArchiveProperties = 0x02,
		kMainStreamsInfo = 0x04,
		kFilesInfo = 0x05,
		kPackInfo = 0x06,
		kUnPackInfo = 0x07,
		kSubStreamsInfo = 0x08,
		kSize = 0x09,
		kCRC = 0x0A,
		kFolder = 0x0B,
		kCodersUnPackSize = 0x0C,
		kNumUnPackStream = 0x0D,
		kName = 0x11,
		kMTime = 0x14,
		kWinAttributes = 0x15,
		kEncodedHeader = 0x17,
		kDummy = 0x19,
	};

	class SevenZip_FileSystem : public StdXX::ContainerFileSystem
	{
		struct Coder
		{
			bool compressed;
			StdXX::CompressionAlgorithm method;
			StdXX::Optional<StdXX::FixedArray<byte>> properties;
		};

		struct Folder
		{
			StdXX::DynamicArray<Coder> coders;
		};

		struct CodersInfo
		{
			struct FolderInfo
			{
				Folder folder;
				uint64 uncompressedSize;
			};

			StdXX::DynamicArray<FolderInfo> folderInfos;
		};

		struct PackInfo
		{
			struct PackedStreamInfo
			{
				uint64 compressedSize;
				StdXX::Optional<uint32> crc32;
			};

			uint64 offset;
			StdXX::DynamicArray<PackedStreamInfo> packedStreams;
		};

		struct StreamsInfo
		{
			StdXX::Optional<PackInfo> packInfo;
			StdXX::Optional<CodersInfo> codersInfo;
		};
	public:
		//Constructor
		inline SevenZip_FileSystem(const StdXX::FileSystemFormat *format, const StdXX::Path &fileSystemPath) : StdXX::ContainerFileSystem(format, fileSystemPath)
		{
			if (!this->containerInputStream.IsNull())
				this->ReadFileSystemHeader();
		}

		//Methods
		void Flush() override;
		StdXX::AutoPointer<const StdXX::FileSystemNode> GetNode(const StdXX::Path &path) const override;

	private:
		//Members
		/** All offsets are based on after the signature header.
		 *
		 */
		uint64 baseOffset;

		//Methods
		StdXX::CompressionAlgorithm MapCodecId(byte(&codecId)[16], uint8 codecIdSize) const;
		uint8 ReadArchiveProperties();
		void ReadCodersInfo(CodersInfo& codersInfo, StdXX::InputStream& inputStream);
		void ReadDigests(uint64 nStreams, StdXX::InputStream& inputStream);
		void ReadFilesInfo(StdXX::InputStream& inputStream);
		void ReadFolder(Folder& folder, StdXX::InputStream& inputStream);
		void ReadHeader(uint64 offset, uint64 size);
		void ReadHeaderData(StdXX::InputStream& inputStream);
		void ReadFileSystemHeader();
		void ReadPackInfo(PackInfo& packInfo, StdXX::InputStream& inputStream);
		void ReadStreamsInfo(StreamsInfo& streamsInfo, StdXX::InputStream& inputStream);
		void ReadSubStreamsInfo(const StreamsInfo& streamsInfo, StdXX::InputStream& inputStream);
		uint64 ReadVariableLengthUInt(StdXX::InputStream& inputStream);
	};
}
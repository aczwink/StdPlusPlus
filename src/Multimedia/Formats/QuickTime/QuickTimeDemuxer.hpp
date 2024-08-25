/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Multimedia/Demuxer.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
#include <Std++/Streams/LimitedInputStream.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Multimedia;

/**
 * Sources:
 * https://developer.apple.com/documentation/quicktime-file-format
 */
class QuickTimeDemuxer : public Demuxer
{
	struct AtomHeader
	{
		//State
		uint32 atomType;
		uint32 dataSize;
		UniquePointer<InputStream> dataInputStream;
		UniquePointer<class DataReader> dataReader;

		//Properties
		inline class DataReader& DataReader()
		{
			return *this->dataReader;
		}
	};

	enum Type
	{
		QuickTime,
		IsoBase
	};
public:
	//Constructor
	inline QuickTimeDemuxer(const ContainerFormat& refFormat, SeekableInputStream& refInput) : Demuxer(refFormat, refInput)
	{
	}

	//Methods
	void ReadHeader() override;
	void Seek(uint64 timestamp, const class TimeScale &timeScale) override;

private:
	//State
	Type type;

	//Methods
	void ReadFileTypeAtom(DataReader& dataReader);
	void ReadMoovAtom(DataReader& dataReader);
	bool ReadNextAtomHeader(InputStream& inputStream, AtomHeader& atomHeader);
	void ReadMetaAtom(DataReader& dataReader);
	void ReadMetaDataListAtom(DataReader &dataReader);
	UniquePointer<IPacket> ReadPacket() override;
	void ReadUserDataAtom(DataReader& dataReader);

	//Inline
	inline String AtomTypeToString(uint32 atomType)
	{
		BufferInputStream bufferInputStream(&atomType, sizeof(atomType));
		TextReader latin1Reader(bufferInputStream, TextCodecType::Latin1);
		return latin1Reader.ReadString(4);
	}

	inline uint32 ReadFourCC(DataReader& dataReader)
	{
		return this->ReadFourCC(dataReader.InputStream());
	}

	inline uint32 ReadFourCC(InputStream& inputStream)
	{
		DataReader fourccReader(false, inputStream);
		return fourccReader.ReadUInt32();
	}

	inline bool ReadNextAtomHeader(DataReader& dataReader, AtomHeader& atomHeader)
	{
		return this->ReadNextAtomHeader(dataReader.InputStream(), atomHeader);
	}
};
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
#include <Std++/Streams/Checksum/CheckedInputStream.hpp>

namespace StdXX
{
	namespace EBML
	{
		class MasterReader
		{
		public:
			//Constructors
			inline MasterReader(const Element& masterElement, InputStream& inputStream) : inputStream(inputStream), inUseInputStream(&inputStream), masterElement(masterElement),
				leftSize(masterElement.dataSize)
			{
			}

			//Inline
			inline InputStream& GetInUseInputStream()
			{
				return *this->inUseInputStream;
			}

			inline uint64 GetRemainingBytes() const
			{
				return this->leftSize;
			}

			inline bool HasMore() const
			{
				return this->leftSize != 0;
			}

			inline void RawBytesReadFromStream(uint32 size)
			{
				ASSERT(size <= this->leftSize, u8"Report please!");
				this->leftSize -= size;
			}

			inline void ReadCurrentChildData(Element& child)
			{
				ReadElementData(child, *this->inUseInputStream);
				ASSERT(child.dataSize <= this->leftSize, u8"Report please!");
				this->leftSize -= child.dataSize;
			}

			inline void ReadNextChildHeader(Element& child)
			{
				ParseElementHeader(child, *this->inUseInputStream);
				ASSERT(child.headerSize <= this->leftSize, u8"Report please!");
				this->leftSize -= child.headerSize;

				if (child.id == EBML_ID_CRC32)
				{
					ASSERT(this->leftSize + 2 == this->masterElement.dataSize, u8"CRC-32 element must be the first one.");
					ASSERT(&this->inputStream == this->inUseInputStream, u8"Can't have multiple CRC-32 elements in master.");

					this->ReadCurrentChildData(child);
					uint32 readCRC32 = child.data.ui;
					
					this->checkedInputStream = new CheckedInputStream(this->inputStream, ChecksumAlgorithm::CRC32, readCRC32);
					this->inUseInputStream = this->checkedInputStream.operator->();

					this->ReadNextChildHeader(child);
				}
			}

			inline void SkipCurrentChildData(Element& child)
			{
				this->inUseInputStream->Skip(child.dataSize);
				ASSERT(child.dataSize <= this->leftSize, u8"Report please!");
				this->leftSize -= child.dataSize;
			}

			inline void Verify()
			{
				if (!this->checkedInputStream.IsNull())
				    this->checkedInputStream->Finish();
			}

		private:
			//Members
			InputStream& inputStream;
			InputStream* inUseInputStream;
			UniquePointer<CheckedInputStream> checkedInputStream;
			const Element& masterElement;
			uint64 leftSize;
		};
	}
}
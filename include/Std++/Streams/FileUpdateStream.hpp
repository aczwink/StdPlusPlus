/*
 * Copyright (c) 2019-2020 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/FileSystem/Path.hpp>
#include "UpdateStream.hpp"

namespace StdXX
{
	class FileUpdateStream : public UpdateStream
	{
	public:
		//Constructor
		/**
		 * Already existing file is opened in update mode. It is never truncated.
		 * If the file does not exist, an error occurs.
		 * */
		FileUpdateStream(const FileSystem::Path &path);

		//Destructor
		~FileUpdateStream();

		//Methods
		void Flush() override;
		uint32 GetBytesAvailable() const override;
		uint64 GetCurrentOffset() const override;
		uint64 QueryRemainingBytes() const override;
		uint64 QuerySize() const override;
		bool IsAtEnd() const override;
		uint32 ReadBytes(void *destination, uint32 count) override;
		void SeekTo(uint64 offset) override;
		uint32 Skip(uint32 nBytes) override;
		uint32 WriteBytes(const void *source, uint32 size) override;

	private:
		//Members
		union
		{
			int fileHandle;
		};
	};
}
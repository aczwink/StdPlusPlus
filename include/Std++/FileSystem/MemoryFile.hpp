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
//Local
#include <Std++/Containers/FIFOBuffer.hpp>
#include "File.hpp"

namespace StdXX::FileSystem
{
	/**
	 * A file that is completely stored in memory.
	 */
	class MemoryFile : public File
	{
	public:
		//Methods
		void ChangePermissions(const FileSystem::NodePermissions &newPermissions) override;
		uint64 GetSize() const override;
		UniquePointer<InputStream> OpenForReading(bool verify) const override;
		UniquePointer<OutputStream> OpenForWriting() override;
		FileSystemNodeInfo QueryInfo() const override;

	private:
		//Members
		FIFOBuffer buffer;
	};
}
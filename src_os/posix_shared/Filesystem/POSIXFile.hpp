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
#include <Std++/FileSystem/File.hpp>
#include "PosixNode.hpp"

namespace _stdxx_
{
	class POSIXFile : public StdXX::FileSystem::File, public PosixNode
	{
	public:
		//Constructor
		inline POSIXFile(const StdXX::FileSystem::Path& path) : PosixNode(path)
		{
		}

		//Methods
		uint64 GetSize() const override;
		StdXX::UniquePointer<StdXX::InputStream> OpenForReading(bool verify) const override;
		StdXX::UniquePointer<StdXX::OutputStream> OpenForWriting() override;
	};
}
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
//Local
#include <Std++/Containers/Map/Map.hpp>
#include "Directory.hpp"

namespace StdXX::FileSystem
{
	/**
	 * A directory that is completely stored in memory.
	 */
	class MemoryDirectory : public Directory
	{
	public:
		//Methods
		bool Exists(const Path &path) const override;
		AutoPointer<Node> GetChild(const String &name) override;
		AutoPointer<const Node> GetChild(const String &name) const override;
		bool IsEmpty() const override;
		NodeInfo QueryInfo() const override;

		//For range-based loop
		DirectoryIterator begin() const override;
		DirectoryIterator end() const override;

		//Inline
		inline void AddChild(const String& nodeName, Node* node)
		{
			ASSERT(!this->children.Contains(nodeName), u8"Can't add child that already exists.");
			this->children[nodeName] = node;
		}

	private:
		//Members
		Map<String, AutoPointer<Node>> children;
	};
}
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
//Class header
#include <Std++/FileSystem/MemoryDirectory.hpp>
//Local
#include <Std++/_Backends/DirectoryIteratorState.hpp>
#include <Std++/FileSystem/DirectoryIterator.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

//Public methods
bool MemoryDirectory::Exists(const Path &path) const
{
	Path leftPart;
	Path child = path.SplitOutmostPathPart(leftPart);
	const String &childString = child.String();
	if(leftPart.String().IsEmpty())
		return this->children.Contains(childString);

	if(this->HasSubDirectory(childString))
		return this->children[childString].Cast<Directory>()->Exists(leftPart);
	return false;
}

AutoPointer<Node> MemoryDirectory::GetChild(const String &name)
{
	if(!this->children.Contains(name))
		return nullptr;
	return this->children[name];
}

AutoPointer<const Node> MemoryDirectory::GetChild(const String &name) const
{
	if(!this->children.Contains(name))
		return nullptr;
	return this->children[name];
}

bool MemoryDirectory::IsEmpty() const
{
	return this->children.IsEmpty();
}

NodeInfo MemoryDirectory::QueryInfo() const
{
	return {
		.size = 0,
		.storedSize = 0,
	};
}

//For range-based loop
DirectoryIterator MemoryDirectory::begin() const
{
	class ContainerDirectoryIteratorState : public _stdxx_::DirectoryIteratorState
	{
	public:
		//Constructor
		ContainerDirectoryIteratorState(const MemoryDirectory &dir)
				: dir(dir), childrenIterator(dir.children.begin())
		{
		}

		//Public methods
		bool Equals(DirectoryIteratorState *other) const override
		{
			if(other == nullptr)
				return this->IsAtEnd();

			ContainerDirectoryIteratorState *otherTyped = dynamic_cast<ContainerDirectoryIteratorState *>(other);
			if(otherTyped)
			{
				return (&this->dir == &otherTyped->dir) &&
				       (this->childrenIterator == otherTyped->childrenIterator);
			}

			return false;
		}

		String GetCurrent() override
		{
			const auto& kv = (*this->childrenIterator);
			return kv.key;
		}

		void Next() override
		{
			++this->childrenIterator;
		}

	private:
		//Members
		const MemoryDirectory &dir;
		ConstMapIterator<String, AutoPointer<Node>> childrenIterator;

		//Inline
		inline bool IsAtEnd() const
		{
			return (this->childrenIterator == this->dir.children.end());
		}
	};

	return DirectoryIterator(new ContainerDirectoryIteratorState(*this));
}

DirectoryIterator MemoryDirectory::end() const
{
	return DirectoryIterator(nullptr);
}
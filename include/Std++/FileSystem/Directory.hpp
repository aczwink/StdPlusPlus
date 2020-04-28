/*
 * Copyright (c) 2018-2020 Amir Czwink (amir130@hotmail.de)
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
#include "../SmartPointers/AutoPointer.hpp"
#include "../SmartPointers/UniquePointer.hpp"
#include "../Streams/OutputStream.hpp"
#include "Node.hpp"
#include "Path.hpp"
#include "File.hpp"

namespace StdXX::FileSystem
{
	//Forward declarations
	class DirectoryWalkerWrapper;

	class STDPLUSPLUS_API Directory : virtual public Node
	{
	public:
		//Destructor
		virtual ~Directory()
		{
		}

		//Abstract
		virtual UniquePointer<OutputStream> CreateFile(const String &name) = 0;
		/**
		 *
		 * @param name
		 * @param permissions - pass null for default permissions
		 */
		virtual void CreateSubDirectory(const String &name, const NodePermissions* permissions = nullptr) = 0;
		virtual bool Exists(const Path &path) const = 0;
		/**
		 * Returns the child identified by name or nullptr if not existent.
		 *
		 * @param name
		 * @return
		 */
		virtual AutoPointer<Node> GetChild(const String &name) = 0;
		virtual AutoPointer<const Node> GetChild(const String &name) const = 0;
		virtual bool IsEmpty() const = 0;

		//Methods
		void CreateDirectoryTree(const Path &directoryPath);
		NodeType GetType() const override;
		DirectoryWalkerWrapper WalkFiles();

		//For range-based loop
		virtual DirectoryIterator begin() const = 0;
		virtual DirectoryIterator end() const = 0;

		//Inline
		inline AutoPointer<Directory> GetSubDirectory(const String& name)
		{
			AutoPointer<Node> node = this->GetChild(name);
			ASSERT(!node.IsNull(), u8"Node does not exist.");
			ASSERT(node->GetType() == NodeType::Directory, u8"Node is not a directory.");
			return node.MoveCast<Directory>();
		}

		inline AutoPointer<const Directory> GetSubDirectory(const String& name) const
		{
			AutoPointer<const Node> node = this->GetChild(name);
			ASSERT(!node.IsNull(), u8"Node does not exist.");
			ASSERT(node->GetType() == NodeType::Directory, u8"Node is not a directory.");
			return node.MoveCast<const Directory>();
		}

		inline bool HasFile(const String& name) const
		{
			AutoPointer<const Node> node = this->GetChild(name);
			if(node.IsNull())
				return false;
			return node->GetType() == NodeType::File;
		}

		inline bool HasSubDirectory(const String& name) const
		{
			AutoPointer<const Node> node = this->GetChild(name);
			if(node.IsNull())
				return false;
			return node->GetType() == NodeType::Directory;
		}
	};
}
/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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

//Buffered
#include <Std++/FileSystem/Buffered/ContainerFile.hpp>
#include <Std++/FileSystem/Buffered/ArchiveFileSystem.hpp>
//
#include <Std++/FileSystem/File.hpp>
#include <Std++/FileSystem/Format.hpp>
#include <Std++/FileSystem/OSFileSystem.hpp>
#include <Std++/FileSystem/Path.hpp>
#include <Std++/FileSystem/POSIXPermissions.hpp>
#include <Std++/FileSystem/TempDirectory.hpp>
#include <Std++/FileSystem/RWFileSystem.hpp>
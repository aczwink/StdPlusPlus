/*
 * Copyright (c) 2018-2021 Amir Czwink (amir130@hotmail.de)
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
//Global
#include <cerrno>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
//Local
#include <Std++/FileSystem/POSIXPermissions.hpp>
#include <Std++/Errorhandling/Exceptions/FileNotFoundException.hpp>
#include <Std++/Streams/FileInputStream.hpp>
#include <Std++/Streams/FileOutputStream.hpp>
#include <Std++/Errorhandling/Errors/IllegalCodePathError.hpp>
#include <Std++/Errorhandling/Exceptions/PermissionDeniedException.hpp>
#include "POSIXDirectoryEnumerator.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::FileSystem;

namespace _stdxx_
{
	//Local functions
	template<typename SecondsType, typename NanoSecondsType>
	static DateTime WithNanoSecondsToTime(SecondsType seconds, NanoSecondsType nanoseconds)
	{
		return DateTime::FromUnixTimeStamp(seconds).AddNanoseconds(nanoseconds);
	}

	class POSIXFileSystem : public OSFileSystem
	{
	public:
		//Public methods
		void ChangePermissions(const Path &path, const Permissions &newPermissions) override
		{
			const POSIXPermissions& unixPermissions = dynamic_cast<const POSIXPermissions &>(newPermissions);

			const char* pathPtr = reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData());
			int ret = chmod(pathPtr, unixPermissions.EncodeMode());
			ASSERT(ret == 0, u8"REPORT THIS PLEASE!");

			ret = chown(pathPtr, unixPermissions.userId, unixPermissions.groupId);
			ASSERT_EQUALS(0, ret);
		}

		Optional<Errors::CreateDirectoryError> CreateDirectory(const Path& path, const Permissions* permissions)
		{
			uint32 permissionFlags = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
			if(permissions)
			{
				const POSIXPermissions* posixPermissions = dynamic_cast<const POSIXPermissions *>(permissions);
				ASSERT(posixPermissions, u8"Illegal permissions type");
				permissionFlags = posixPermissions->EncodeMode();
			}
			int ret = mkdir(reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData()), permissionFlags);
			if(ret)
			{
				switch(errno)
				{
					case EACCES:
						return Errors::CreateDirectoryError::PermissionsDenied;
					case EEXIST:
						return Errors::CreateDirectoryError::FileExists;
					case ELOOP:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case EMLINK:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case ENAMETOOLONG:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case ENOENT:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case ENOSPC:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case ENOTDIR:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case EROFS:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
				}
			}

			return {};
		}

		UniquePointer<OutputStream> CreateFile(const Path &filePath) override
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return nullptr;
		}

		void CreateLink(const Path &linkPath, const Path &linkTargetPath) override
		{
			int ret = symlink(
					reinterpret_cast<const char *>(linkTargetPath.String().ToUTF8().GetRawZeroTerminatedData()),
					reinterpret_cast<const char *>(linkPath.String().ToUTF8().GetRawZeroTerminatedData()));
			ASSERT_EQUALS(0, ret);
		}

		void DeleteFile(const Path& path) override
		{
			int ret = unlink(this->AsNativePathString(path));
			if(ret)
			{
				switch(errno)
				{
					case EACCES:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case EBUSY:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case ELOOP:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case ENAMETOOLONG:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case ENOENT:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case ENOTDIR:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case EPERM:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case EROFS:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					case ETXTBSY:
						NOT_IMPLEMENTED_ERROR; //TODO: implement me
					default:
						this->HandleNonCompliantUnlinkError();
				}
			}
		}

		UniquePointer<DirectoryEnumerator> EnumerateChildren(const Path& path) const override
		{
			return new _stdxx_::POSIXDirectoryEnumerator(path);
		}

		void Flush() override
		{
		}

		void Move(const Path &from, const Path &to) override
		{
			int ret = rename(reinterpret_cast<const char *>(from.String().ToUTF8().GetRawZeroTerminatedData()),
							 reinterpret_cast<const char *>(to.String().ToUTF8().GetRawZeroTerminatedData()));
			ASSERT_EQUALS(0, ret);
		}

		UniquePointer<InputStream> OpenFileForReading(const Path& path, bool verify) const
		{
			return new FileInputStream(path);
		}

		UniquePointer<OutputStream> OpenFileForWriting(const Path& path)
		{
			return new FileOutputStream(path);
		}

		Optional<FileInfo> QueryFileInfo(const Path& path) const
		{
			struct stat sb{};
			int ret = lstat(reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData()), &sb);
			if(ret != 0)
			{
				switch(errno)
				{
					case EACCES:
						NOT_IMPLEMENTED_ERROR;
					case EIO:
						NOT_IMPLEMENTED_ERROR;
					case ELOOP:
						NOT_IMPLEMENTED_ERROR;
					case ENAMETOOLONG:
						NOT_IMPLEMENTED_ERROR;
					case ENOENT:
						return {};
					case ENOTDIR:
						NOT_IMPLEMENTED_ERROR;
					case EOVERFLOW:
						NOT_IMPLEMENTED_ERROR;
					default:
						RAISE(ErrorHandling::IllegalCodePathError);
				}
			}

			FileInfo info{};

			//type
			if(S_ISDIR(sb.st_mode))
				info.type = FileType::Directory;
			else if(S_ISLNK(sb.st_mode))
				info.type = FileType::Link;
			else if(S_ISREG(sb.st_mode))
				info.type = FileType::File;
			else
				NOT_IMPLEMENTED_ERROR;

			//set file size
			info.size = static_cast<uint64>(sb.st_size);

			//set last modified time
#ifdef XPC_OS_DARWIN
			info.lastModifiedTime = WithNanoSecondsToTime(sb.st_mtimespec.tv_sec, sb.st_mtimespec.tv_nsec);
#else
			info.lastModifiedTime = WithNanoSecondsToTime(sb.st_mtim.tv_sec, sb.st_mtim.tv_nsec);
#endif

			//set stored size
#if defined(XPC_OS_LINUX) || defined(XPC_OS_DARWIN)
			//sb.st_size can be larger in case the file has "holes"...
			//on linux and darwin st_blocks is in 512 units
			info.storedSize = sb.st_blocks * 512_u64;
#else
			info.storedSize = info.size;
#endif

			//permissions
			info.permissions = new FileSystem::POSIXPermissions(sb.st_uid, sb.st_gid, sb.st_mode);

			return info;
		}

		SpaceInfo QuerySpace() const override
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
			return {};
		}

		Optional<Path> ReadLinkTarget(const Path& path) const
		{
			char buffer[4096];
			ssize_t nBytesInBuffer = readlink(reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData()), buffer, sizeof(buffer));
			ASSERT(nBytesInBuffer != -1, u8"REPORT THIS PLEASE!");
			ASSERT(nBytesInBuffer < sizeof(buffer), u8"REPORT THIS PLEASE!");

			return {String::CopyUtf8Bytes(reinterpret_cast<const uint8 *>(buffer), nBytesInBuffer)};
		}

		void RemoveDirectory(const Path &path) override
		{
			int ret = rmdir(this->AsNativePathString(path));
			if(ret)
			{
				NOT_IMPLEMENTED_ERROR; //TODO: implement me
			}
		}

	private:
		//Abstract
		virtual void HandleNonCompliantUnlinkError()
		{
			NOT_IMPLEMENTED_ERROR; //TODO: implement me
		}

		//Inline
		inline const char* AsNativePathString(const Path& path) const
		{
			return reinterpret_cast<const char *>(path.String().ToUTF8().GetRawZeroTerminatedData());
		}
	};
}
set(SOURCE_FILES
	${SOURCE_FILES}

	#Buffered
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/ArchiveCreationFileSystem.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/ArchiveFileSystem.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/BufferedMetadataFileSystem.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/ContainerFile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/EmbeddedFileInputStream.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/FileMetadataNode.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/MemoryDirectory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/MemoryFile.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/DirectoryEnumerator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/DirectoryWalker.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/File.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileSystemsManager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileInfo.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ReadableFileSystem.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ReadOnlyFile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Path.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/TempDirectory.hpp

	PARENT_SCOPE)
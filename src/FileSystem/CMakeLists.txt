add_subdirectory(Formats)

set(SOURCE_FILES
	${SOURCE_FILES}

	#Buffered
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/BufferedMetadataFileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/BufferedWritableMetadataFileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/ArchiveFileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Buffered/WritableContainerFileSystem.cpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/WritableContainerDirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ContainerFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EmbeddedFileInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileSystemsManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DirectoryWalker.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MemoryDirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Path.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/POSIXPermissions.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/WritableFileSystem.cpp

	PARENT_SCOPE)
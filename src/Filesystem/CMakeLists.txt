add_subdirectory(Formats)

set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/BufferedMetadataFileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ContainerDirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ContainerFile.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/EmbeddedFileInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ContainerFileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Directory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DirectoryWalker.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/File.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileSystemFormat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Link.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MemoryDirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MemoryFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Path.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/URL.cpp

	PARENT_SCOPE)
add_subdirectory(Formats)

set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/ContainerDirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ContainerFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ContainerFileInputStream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ContainerFileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Directory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DirectoryWalker.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FileSystemFormat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Path.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/URL.cpp

	PARENT_SCOPE)
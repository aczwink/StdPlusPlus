set(SOURCE_FILES
	${SOURCE_FILES}

	#7zip
	${CMAKE_CURRENT_SOURCE_DIR}/7zip/7zip_FileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/7zip/7zip_FileSystem.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/7zip/7zip_FileSystemFormat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/7zip/7zip_FileSystemFormat.hpp

	#gzip

	#zip
	${CMAKE_CURRENT_SOURCE_DIR}/zip/AEDecrypter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/AEDecrypter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/CentralDirectoryRecord.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/CentralDirectoryRecord.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/EndOfCentralDirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/EndOfCentralDirectory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/LocalFileHeader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/LocalFileHeader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/Zip.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipDirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipDirectory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipFile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipFileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipFileSystem.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipFileSystemFormat.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipFileSystemFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipLink.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipLink.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipReadableFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/zip/ZipReadableFile.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/__Register.cpp

	PARENT_SCOPE)
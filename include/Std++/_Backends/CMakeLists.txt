add_subdirectory(UI)

set(SOURCE_FILES
	${SOURCE_FILES}
	#.
	${CMAKE_CURRENT_SOURCE_DIR}/AudioDevice.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Backend.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BackendManager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ComputeDevice.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/DeviceEnumeratorState.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/DirectoryIteratorState.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/MultimediaBackend.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenderBackend.hpp

	PARENT_SCOPE)
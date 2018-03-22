set(SOURCE_FILES
	${SOURCE_FILES}
	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Backend.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BackendManager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ComputeBackend.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/DeviceEnumeratorState.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/DirectoryIteratorState.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenderBackend.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UIBackend.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/WindowBackend.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/WindowBackendType.hpp

	PARENT_SCOPE)
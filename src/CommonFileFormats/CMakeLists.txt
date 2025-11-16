add_subdirectory(CSV)
add_subdirectory(JSON)
add_subdirectory(PDF)
add_subdirectory(XML)

set(SOURCE_FILES
	${SOURCE_FILES}

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/ConfigurationFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/YAML.cpp

	PARENT_SCOPE)
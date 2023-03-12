set(SOURCE_FILES
	${SOURCE_FILES}

	#OpenType
	${CMAKE_CURRENT_SOURCE_DIR}/OpenType/OpenTypeFace.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OpenType/OpenTypeFace.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/OpenType/PostscriptTypeface.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OpenType/PostscriptTypeface.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/OpenType/TrueTypeFace.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/OpenType/TrueTypeFace.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Typeface.cpp

	PARENT_SCOPE)
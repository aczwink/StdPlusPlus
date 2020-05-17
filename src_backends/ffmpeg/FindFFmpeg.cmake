find_path(AVCODEC_INCLUDE_DIR
	NAMES libavcodec/avcodec.h
	PATHS ${AVCODEC_INCLUDE_DIRS}
	/usr/include
	/usr/local/include
	/opt/local/include
	/sw/include
	C:/cmake_opt/include
	PATH_SUFFIXES . .
)

find_library(LIBAVCODEC
	NAMES avcodec
	PATHS ${AVCODEC_LIBRARY_DIRS}
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	/sw/lib
	C:/cmake_opt/lib
)

find_library(LIBAVFORMAT
	NAMES avformat
	PATHS ${AVFORMAT_LIBRARY_DIRS}
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	/sw/lib
	C:/cmake_opt/lib
)

find_library(LIBAVUTIL
	NAMES avutil
	PATHS ${AVUTIL_LIBRARY_DIRS}
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	/sw/lib
	C:/cmake_opt/lib
)

if (LIBAVCODEC AND LIBAVFORMAT)
	set(LIBAV_FOUND TRUE)
endif()

if (LIBAV_FOUND)
	set(LIBAV_INCLUDE_DIR ${AVCODEC_INCLUDE_DIR})
	set(LIBAV_LIBRARIES ${LIBAVCODEC} ${LIBAVFORMAT} ${LIBAVUTIL})
endif (LIBAV_FOUND)
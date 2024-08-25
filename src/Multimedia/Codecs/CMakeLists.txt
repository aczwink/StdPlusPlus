set(SOURCE_FILES
	${SOURCE_FILES}

	#Images
	${CMAKE_CURRENT_SOURCE_DIR}/Images/PNG_CodingFormat.hpp

	#MPEG
	${CMAKE_CURRENT_SOURCE_DIR}/MPEG/AAC_CodingFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/MPEG/MP3/MP3_CodingFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/MPEG/H264_CodingFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/MPEG/MPEG4_Part2_CodingFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/MPEG/MS_MPEG4_Part2_V2_CodingFormat.hpp

	#PCM
	${CMAKE_CURRENT_SOURCE_DIR}/PCM/ADPCM/MS_ADPCM_CodingFormat.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/PCM/Float32LE/PCM_Float32LE_CodingFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PCM/Float32LE/PCM_Float32LE_Decoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PCM/Float32LE/PCM_Float32LE_Encoder.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/PCM/S16/PCM_S16BE_CodingFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PCM/S16/PCM_S16_Decoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PCM/S16/PCM_S16_Encoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PCM/S16/PCM_S16LE_CodingFormat.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/PCM/S8/PCM_S8_CodingFormat.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/PCM/U8/PCM_U8_CodingFormat.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/PCM/PCM_DecoderContext.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PCM/PCM_EncoderContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PCM/PCM_EncoderContext.hpp

	#Raw_Subtitles
	${CMAKE_CURRENT_SOURCE_DIR}/Raw_Subtitles/UTF8PlainText_CodingFormat.hpp

	#RawSinglePlaneVideo
	${CMAKE_CURRENT_SOURCE_DIR}/RawSinglePlaneVideo/RawSinglePlaneVideo_CodingFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/RawSinglePlaneVideo/RawSinglePlaneVideoDecoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/RawSinglePlaneVideo/RawSinglePlaneVideoDecoderContext.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/RawSinglePlaneVideo/RawSinglePlaneVideoEncoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/RawSinglePlaneVideo/RawSinglePlaneVideoEncoderContext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RawSinglePlaneVideo/RawSinglePlaneVideoEncoderContext.hpp

	#RGB24
	${CMAKE_CURRENT_SOURCE_DIR}/RGB24/RGB24_Codec.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/RGB24/RGB24_Encoder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RGB24/RGB24_Encoder.hpp

	#S3TC
	${CMAKE_CURRENT_SOURCE_DIR}/S3TC/S3TC_DXT1_CodingFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/S3TC/S3TC_DXT1_Decoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/S3TC/S3TC_DXT1_DecoderContext.hpp

	#Xiph
	${CMAKE_CURRENT_SOURCE_DIR}/Xiph/Theora_CodingFormat.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Xiph/Vorbis_CodingFormat.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/AC3_CodingFormat.hpp

	PARENT_SCOPE)
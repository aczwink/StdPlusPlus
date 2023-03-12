set(SOURCE_FILES
	${SOURCE_FILES}

	#LanguageProcessors
	${CMAKE_CURRENT_SOURCE_DIR}/LanguageProcessors/ArabicLanguageProcessor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LanguageProcessors/ArabicLanguageProcessor.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/LanguageProcessors/LatinLanguageProcessor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LanguageProcessors/LatinLanguageProcessor.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Catalog.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CIDFontType2.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CrossReferenceTable.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Dictionary.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Document.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Font.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FontDescriptor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Graphics.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/IndirectObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/IndirectReference.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/IntObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LanguageProcessor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LanguageProcessor.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/NameObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Page.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PageTree.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StreamObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Type0Font.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UnicodeMap.cpp

	PARENT_SCOPE)
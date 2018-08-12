set(SOURCE_FILES
	${SOURCE_FILES}

	#Array
	${CMAKE_CURRENT_SOURCE_DIR}/Array/ArrayIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Array/ConstArrayIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Array/DynamicArray.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Array/FixedArray.hpp

	#FiniteSet
	${CMAKE_CURRENT_SOURCE_DIR}/FiniteSet/FiniteSet.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FiniteSet/FiniteSetIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/FiniteSet/FiniteSetNode.hpp

	#LinkedList
	${CMAKE_CURRENT_SOURCE_DIR}/LinkedList/LinkedList.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/LinkedList/LinkedListConstIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/LinkedList/LinkedListIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/LinkedList/LinkedListNode.hpp

	#Map
	${CMAKE_CURRENT_SOURCE_DIR}/Map/BijectiveMap.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Map/Map.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Map/MapIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Map/MapNode.hpp

	#Strings
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/ByteString.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/ConstStringIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/FixedCharLengthString.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/VariableCharLengthString.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/OldString.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF32String.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/ResizeableString.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/String.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/StringUtil.h
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF-8/ConstUTF8StringIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF-8/UTF8String.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF-16/UTF16String.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF-16/CConstUTF16StringIterator.h

	#Table
	${CMAKE_CURRENT_SOURCE_DIR}/Table/FixedTable.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/FIFOBuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Container.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/KeyValuePair.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PriorityQueue.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ResizeableSequenceContainer.hpp

	PARENT_SCOPE)
set(SOURCE_FILES
	${SOURCE_FILES}

	#Array
	${CMAKE_CURRENT_SOURCE_DIR}/Array/ArrayIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Array/ConstArrayIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Array/DynamicArray.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Array/FixedArray.hpp

	#BinaryTreeSet
	${CMAKE_CURRENT_SOURCE_DIR}/BinaryTreeSet/BinaryTreeSet.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BinaryTreeSet/FiniteSetIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/BinaryTreeSet/FiniteSetNode.hpp

	#HashMap
	${CMAKE_CURRENT_SOURCE_DIR}/HashMap/HashMap.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/HashMap/HashTableEntry.hpp

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
		${CMAKE_CURRENT_SOURCE_DIR}/Strings/ConstStringIterator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/FixedCharLengthString.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/VariableCharLengthString.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Strings/UTF32String.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/ResizeableString.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/String.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Strings/StringUtil.h

		#Table
	${CMAKE_CURRENT_SOURCE_DIR}/Table/FixedTable.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/FIFOBuffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Container.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/KeyValuePair.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/OrderedRangeSet.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PriorityQueue.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ResizeableSequenceContainer.hpp

	PARENT_SCOPE)
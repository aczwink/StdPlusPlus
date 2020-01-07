set(SOURCE_FILES
	${SOURCE_FILES}

	#Distributions
	${CMAKE_CURRENT_SOURCE_DIR}/Distributions/UniformRealDistribution.hpp

	#RandomBitGenerators
	${CMAKE_CURRENT_SOURCE_DIR}/RandomBitGenerators/LinearCongruentialGenerator.hpp

	#Vector2
	${CMAKE_CURRENT_SOURCE_DIR}/Vector2/Vector2.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Vector2/Vector2.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Vector2/Vector2_scalar.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Vector2/Vector2_SSE.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Vector2/Vector2Base_scalar.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Vector2/Vector2Base_SSE.inl

	#Vector4
	${CMAKE_CURRENT_SOURCE_DIR}/Vector4/Vector4.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Vector4/Vector4.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Vector4/Vector4_scalar.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Vector4/Vector4_SSE.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Vector4/Vector4Base_scalar.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Vector4/Vector4Base_SSE.inl

	#Geometry
	${CMAKE_CURRENT_SOURCE_DIR}/Geometry/AxisAlignedBox.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Geometry/ObjectIntersection.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Geometry/Rect.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Degree.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Fraction.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Integer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Matrix2x2.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Matrix3x3.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Matrix4x4.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Natural.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Point.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Quaternion.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Radian.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/RandomBitGenerator.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Range.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Size.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Vector3.hpp

	PARENT_SCOPE)
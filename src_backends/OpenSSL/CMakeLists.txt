option(EXTENSION_OPENSSL "OpenSSL cryptography-extension." OFF)



if(${BACKENDS_AUTO_AVAILABLE})
	find_package(OpenSSL)
	if(APPLE AND NOT OPENSSL_FOUND) #apple deprecated openssl in favor of their own api-.-
		set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl") #brew installs it here
		find_package(OpenSSL)
	endif()

	if (OPENSSL_FOUND)
		set(EXTENSION_OPENSSL ON)
	endif ()
endif ()



if(${EXTENSION_OPENSSL})
	find_package(OpenSSL)
	if(APPLE AND NOT OPENSSL_FOUND) #apple deprecated openssl in favor of their own api-.-
		set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl") #brew installs it here
		find_package(OpenSSL)
	endif()

	find_package(OpenSSL REQUIRED)

	set(SOURCE_FILES
		${SOURCE_FILES}

		#Cryptography
		${CMAKE_CURRENT_SOURCE_DIR}/Cryptography/OpenSSL_BlockCipher.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Cryptography/OpenSSL_BlockCipher.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Cryptography/OpenSSL_BlockDecipher.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Cryptography/OpenSSL_BlockDecipher.hpp
		${CMAKE_CURRENT_SOURCE_DIR}/Cryptography/OpenSSL_Hasher.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Cryptography/OpenSSL_Hasher.hpp

		#.
		${CMAKE_CURRENT_SOURCE_DIR}/OpenSSL_Extension.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/OpenSSL_Extension.hpp
		)

	target_compile_definitions(Std++ PRIVATE _STDXX_EXTENSION_OPENSSL)
	target_include_directories(Std++ PRIVATE ${OPENSSL_INCLUDE_DIR})
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY})

	#set in parent scope
	set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
	set(STDPLUSPLUS_PRIVATE_LIBRARIES ${STDPLUSPLUS_PRIVATE_LIBRARIES} PARENT_SCOPE)
endif ()
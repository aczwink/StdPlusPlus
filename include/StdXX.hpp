/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of Std++.
 *
 * Std++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Std++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Std++.  If not, see <http://www.gnu.org/licenses/>.
 */

//Audio
#include <Std++/Audio/Buffer.hpp>
#include <Std++/Audio/Source.hpp>

//CommonFileFormats
//CommonFileFormats/CSV
#include <Std++/CommonFileFormats/CSV/CSVReader.hpp>
#include <Std++/CommonFileFormats/CSV/CSVWriter.hpp>
//CommonFileFormats/PDF
#include <Std++/CommonFileFormats/PDF/Document.hpp>
//CommonFileFormats/XML
#include <Std++/CommonFileFormats/XML/Document.hpp>
#include <Std++/CommonFileFormats/XML/TextNode.hpp>

//Compute
#include <Std++/Compute/CommandQueue.hpp>
#include <Std++/Compute/DeviceContext.hpp>

//Cryptography
#include <Std++/Cryptography/BlockCipher.hpp>
#include <Std++/Cryptography/BlockDecipher.hpp>
#include <Std++/Cryptography/CBCCipher.hpp>
#include <Std++/Cryptography/CBCDecipher.hpp>
#include <Std++/Cryptography/CheckedHashingInputStream.hpp>
#include <Std++/Cryptography/CTRCipher.hpp>
#include <Std++/Cryptography/CTRDecipher.hpp>
#include <Std++/Cryptography/DefaultCounter.hpp>
#include <Std++/Cryptography/ECBCipher.hpp>
#include <Std++/Cryptography/ECBDecipher.hpp>
#include <Std++/Cryptography/HMAC.hpp>
#include <Std++/Cryptography/KeyDerivationFunctions.hpp>
#include <Std++/Cryptography/SecureRandomNumberGenerator.hpp>

//Debugging
#include <Std++/Debugging/DebugOutputStreams.hpp>

//Devices
#include <Std++/Devices/AudioDevice.hpp>
#include <Std++/Devices/ComputeDevice.hpp>
#include <Std++/Devices/DeviceEnumerator.hpp>

//Internationalization
#include <Std++/Internationalization/TimeZone.hpp>

//Memory
#include <Std++/Memory/StaticMemoryBlockAllocator.hpp>

//CodeAnalysis
#include <Std++/CodeAnalysis/Program.hpp>

//Compression
#include <Std++/Compression/Compressor.hpp>
#include <Std++/Compression/Decompressor.hpp>
#include <Std++/Compression/DictionaryDecompressor.hpp>
#include <Std++/Compression/HuffmanDecoder.hpp>
#include <Std++/Compression/IndexedSlidingDictionary.hpp>
#include <Std++/Compression/SlidingDictionary.hpp>

//Debugging
#include <Std++/Debugging/FunctionHook.hpp>

//Formal languages
#include <Std++/FormalLanguages/CharacterClass.hpp>
#include <Std++/FormalLanguages/NFA.hpp>
#include <Std++/FormalLanguages/RegEx.hpp>
#include <Std++/FormalLanguages/CRegExParser.h>

//Internationalization
#include <Std++/Internationalization/Locale.hpp>

//Multitasking
#include <Std++/Multitasking/ConditionVariable.hpp>
#include <Std++/Multitasking/Mutex.hpp>
#include <Std++/Multitasking/CProcess.h>
#include <Std++/Multitasking/SpinLock.hpp>
#include <Std++/Multitasking/StaticThreadPool.hpp>
#include <Std++/Multitasking/Thread.hpp>
#include <Std++/Multitasking/Multitasking.hpp>

//Network
#include <Std++/Network/DatagramSocket.hpp>
#include <Std++/Network/HTTPServer.hpp>
#include <Std++/Network/IPv4Address.hpp>
#include <Std++/Network/IPv4Range.hpp>
#include <Std++/Network/TCPServerSocket.hpp>

//Rendering
#include <Std++/Rendering/DeviceContext.hpp>
#include <Std++/Rendering/InputLayout.hpp>
#include <Std++/Rendering/ICubeMap.h>
#include <Std++/Rendering/IFrameBuffer.h>
#include <Std++/Rendering/IndexBuffer.hpp>
#include <Std++/Rendering/InputState.hpp>
#include <Std++/Rendering/Shader.hpp>
#include <Std++/Rendering/ShaderProgram.hpp>
#include <Std++/Rendering/Texture2D.hpp>
#include <Std++/Rendering/VertexBuffer.hpp>
//Rendering/VectorPath
#include <Std++/Rendering/VectorPath/DevicePathRenderer.hpp>

//Serialization
#include <Std++/Serialization/JSONDeserializer.hpp>
#include <Std++/Serialization/JSONSerializer.hpp>
#include <Std++/Serialization/StandardSerializations.hpp>
#include <Std++/Serialization/StringMapping.hpp>
#include <Std++/Serialization/XMLDeserializer.hpp>
#include <Std++/Serialization/XmlSerializer.hpp>

//Smart Pointers
#include <Std++/SmartPointers/UniquePointer.hpp>

//Time
#include <Std++/Time/Clock.hpp>
#include <Std++/Time/DateTime.hpp>
#include <Std++/Time/Time.hpp>
#include <Std++/Time/TimeMisc.hpp>
#include <Std++/Time/Timer.hpp>


//.
#include <Std++/__InitAndShutdown.h>
#include <Std++/Char.hpp>
#include <Std++/CommonFileFormats/ConfigurationFile.hpp>
#include <Std++/Function.hpp>
#include <Std++/Streams/Checksum/ChecksumFunction.hpp>
#include <Std++/Tuple.hpp>
#include <Std++/Float.hpp>
#include <Std++/Signed.hpp>
#include <Std++/Mathematics.hpp>
#include <Std++/Containers/Optional.hpp>
#include <Std++/ShutdownManager.hpp>


#include "StdXXCommandLine.hpp"
#include "StdXXCore.hpp"
#include "StdXXErrorHandling.hpp"
#include "StdXXEventHandling.hpp"
#include "StdXXFileSystem.hpp"
#include "StdXXMath.hpp"
#include "StdXXMIDI.hpp"
#include "StdXXMultimedia.hpp"
#include "StdXXStreams.hpp"
#include "StdXXUI.hpp"

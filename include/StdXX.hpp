/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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

//Filetypes
#include <Std++/Filetypes/UTIConstants.hpp>

//Internationalization
#include <Std++/Internationalization/TimeZone.hpp>

//Mathematics
#include <Std++/Math/Degree.hpp>
#include <Std++/Math/Integer.hpp>
#include <Std++/Math/Natural.hpp>
#include <Std++/Math/RandomBitGenerator.hpp>
#include <Std++/Math/Quaternion.hpp>
#include <Std++/Math/Radian.hpp>
#include <Std++/Math/Size.hpp>
#include <Std++/Math/Matrix3x3.hpp>
#include <Std++/Math/Matrix4x4.hpp>
#include <Std++/Math/Vector2/Vector2.hpp>
#include <Std++/Math/Vector2/Vector2.inl>
#include <Std++/Math/Vector3.hpp>
#include <Std++/Math/Vector4/Vector4.inl>

//CodeAnalysis
#include <Std++/CodeAnalysis/Program.hpp>

//Compression
#include <Std++/Compression/HuffmanDecoder.hpp>
#include <Std++/Compression/Compressor.hpp>
#include <Std++/Compression/Decompressor.hpp>
#include <Std++/Compression/IndexedSlidingDictionary.hpp>
#include <Std++/Compression/SlidingDictionary.hpp>

//Debugging
#include <Std++/Debugging/FunctionHook.hpp>

//Formal languages
#include <Std++/FormalLanguages/RegEx.hpp>
#include <Std++/FormalLanguages/CRegExParser.h>

//Internationalization
#include <Std++/Internationalization/Locale.hpp>

//Math
#include <Std++/Math/CComplex.h>
#include <Std++/Math/Rational.hpp>
#include <Std++/Math/CMatrix.h>
//Math/Distributions
#include <Std++/Math/Distributions/UniformRealDistribution.hpp>
//Math/Geometry
#include <Std++/Math/Geometry/AxisAlignedBox.hpp>
#include <Std++/Math/Geometry/ObjectIntersection.hpp>
//Math/RandomBitGenerators
#include <Std++/Math/RandomBitGenerators/LinearCongruentialGenerator.hpp>

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
#include <Std++/Serialization/XmlDeserializer.hpp>
#include <Std++/Serialization/XmlSerializer.hpp>

//Smart Pointers
#include <Std++/SmartPointers/UniquePointer.hpp>

//Time
#include <Std++/Time/Clock.hpp>
#include <Std++/Time/DateTime.hpp>
#include <Std++/Time/Time.hpp>
#include <Std++/Time/TimeMisc.hpp>
#include <Std++/Time/Timer.hpp>

//UI
#include <Std++/UI/Action.hpp>
#include <Std++/UI/CommonDialogs.hpp>
#include <Std++/UI/CTerminalEventQueue.h>
#include <Std++/UI/Keyboard.hpp>
#include <Std++/UI/MainAppWindow.hpp>
#include <Std++/UI/Mouse.hpp>
//Containers
#include <Std++/UI/Containers/CTabBox.h>
#include <Std++/UI/Containers/CompositeWidget.hpp>
#include <Std++/UI/Containers/GroupBox.hpp>
#include <Std++/UI/Containers/ScrollArea.hpp>
//Controllers
#include <Std++/UI/Controllers/ACheckListController.h>
#include <Std++/UI/Controllers/ListController.hpp>
#include <Std++/UI/Controllers/TableController.hpp>
#include <Std++/UI/Controllers/TreeController.hpp>
//Controls
#include <Std++/UI/Controls/CheckBox.hpp>
#include <Std++/UI/Views/SelectBox.hpp>
#include <Std++/UI/Controls/Label.hpp>
#include <Std++/UI/Controls/LineEdit.hpp>
#include <Std++/UI/Controls/ProgressBar.hpp>
#include <Std++/UI/Controls/PushButton.hpp>
#include <Std++/UI/Controls/RadioButton.hpp>
#include <Std++/UI/Controls/SearchBox.hpp>
#include <Std++/UI/Controls/Slider.hpp>
#include <Std++/UI/Controls/SpinBox.hpp>
#include <Std++/UI/Controls/TextEdit.hpp>
//Displays
#include <Std++/UI/Displays/VideoWidget.hpp>
//Events
#include <Std++/UI/Events/KeyEvent.hpp>
//Layouts
#include <Std++/UI/Layouts/GridLayout.hpp>
//Menu
#include <Std++/UI/Menu/Menu.hpp>
#include <Std++/UI/Menu/MenuBar.hpp>
//Transfer
#include <Std++/UI/Transfer/CFileTransfer.h>
//Typing
#include <Std++/UI/Typing/ITypeface.h>
//Views
#include <Std++/UI/Displays/PathRenderTargetWidget.hpp>
#include <Std++/UI/Displays/RenderTargetWidget.hpp>
#include <Std++/UI/Views/CCheckListView.h>
#include <Std++/UI/Views/ListView.hpp>
#include <Std++/UI/Views/TableView.hpp>
#include <Std++/UI/Views/TreeView.hpp>

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
#include "StdXXMultimedia.hpp"
#include "StdXXStreams.hpp"
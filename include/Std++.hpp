/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/CommonFileFormats/CSV/CSVWriter.hpp>

//Compute
#include <Std++/Compute/CommandQueue.hpp>
#include <Std++/Compute/DeviceContext.hpp>

//Containers
#include <Std++/Containers/Container.hpp>
//FiniteSet
#include <Std++/Containers/FiniteSet/FiniteSet.hpp>
//FixedArray
#include <Std++/Containers/Array/FixedArray.hpp>
//LinkedList
#include <Std++/Containers/LinkedList/LinkedList.hpp>
//Map
#include <Std++/Containers/Map/Map.hpp>
//Strings
#include <Std++/Containers/Strings/ByteString.hpp>
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Containers/Strings/UTF-16/UTF16String.hpp>
//Tables
#include <Std++/Containers/Table/FixedTable.hpp>
#include <Std++/Containers/FIFOBuffer.hpp>

//Debugging
#include <Std++/Debugging/DebugOutputStreams.hpp>

//Devices
#include <Std++/Devices/AudioDevice.hpp>
#include <Std++/Devices/ComputeDevice.hpp>
#include <Std++/Devices/DeviceEnumerator.hpp>

//Eventhandling
#include <Std++/Eventhandling/StandardEventQueue.hpp>

//Filesystem
#include <Std++/Filesystem/ContainerFileSystem.hpp>
#include <Std++/Filesystem/DirectoryWalker.hpp>
#include <Std++/Filesystem/File.hpp>
#include <Std++/Filesystem/FileSystem.hpp>
#include <Std++/Filesystem/FileSystemFormat.hpp>
#include <Std++/Filesystem/OSFileSystem.hpp>
#include <Std++/Filesystem/Path.hpp>

//Internationalization
#include <Std++/Internationalization/TimeZone.hpp>

//Mathematics
#include <Std++/Math/Degree.hpp>
#include <Std++/Math/Integer.hpp>
#include <Std++/Math/Natural.hpp>
#include <Std++/Math/RandomNumberGenerator.hpp>
#include <Std++/Math/Radian.hpp>
#include <Std++/Math/Size.hpp>
#include <Std++/Math/Matrix3x3.hpp>
#include <Std++/Math/Matrix4x4.hpp>
#include <Std++/Math/Vector2/Vector2.hpp>
#include <Std++/Math/Vector2/Vector2.inl>
#include <Std++/Math/Vector3.hpp>
#include <Std++/Math/Vector4/Vector4.hpp>

//Streams
#include <Std++/Streams/OutputStream.hpp>
#include <Std++/Streams/BufferedInputStream.hpp>
#include <Std++/Streams/BufferInputStream.hpp>
#include <Std++/Streams/BufferOutputStream.hpp>
#include <Std++/Streams/CConcatenatedInputStream.h>
#include <Std++/Streams/FileInputStream.hpp>
#include <Std++/Streams/FileOutputStream.hpp>
#include <Std++/Streams/StdOut.hpp>
#include <Std++/Streams/StdIn.hpp>
//Streams/Bitstreams
#include <Std++/Streams/Bitstreams/CBitInputStream.h>
#include <Std++/Streams/Bitstreams/BitInputStreamBitReversed.hpp>

//Readers
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
//Streams/Writers
#include <Std++/Streams/Writers/DataWriter.hpp>
#include <Std++/Streams/Writers/TextWriter.hpp>

//CodeAnalysis
#include <Std++/CodeAnalysis/CProgram.h>

//Compression
#include <Std++/Compression/CHuffmanDecoder.h>
#include <Std++/Compression/Decompressor.hpp>

//Debugging
#include <Std++/Debugging/CHook.h>

//ErrorHandling
#include <Std++/Errorhandling/Error.hpp>
#include <Std++/Errorhandling/Exception.hpp>

//Filesystem
#include <Std++/Filesystem/Path.hpp>
#include <Std++/Filesystem/DirectoryIterator.hpp>

//Formal languages
#include <Std++/Formal Languages/CRegEx.h>
#include <Std++/Formal Languages/CRegExParser.h>

//Internationalization
#include <Std++/Internationalization/Locale.hpp>

//Math
#include <Std++/Math/CComplex.h>
#include <Std++/Math/Fraction.hpp>
#include <Std++/Math/CMatrix.h>
//Math/Geometry
#include <Std++/Math/Geometry/AxisAlignedBox.hpp>
#include <Std++/Math/Geometry/ObjectIntersection.hpp>

//Multimedia
#include <Std++/Multimedia/AudioBuffer.hpp>
#include <Std++/Multimedia/AudioDecoder.hpp>
#include <Std++/Multimedia/AudioFrame.hpp>
#include <Std++/Multimedia/AudioStream.hpp>
#include <Std++/Multimedia/ComputePixmapResampler.hpp>
#include <Std++/Multimedia/Index.hpp>
#include <Std++/Multimedia/Demuxer.hpp>
#include <Std++/Multimedia/Encoder.hpp>
#include <Std++/Multimedia/Format.hpp>
#include <Std++/Multimedia/MediaPlayer.hpp>
#include <Std++/Multimedia/Muxer.hpp>
#include <Std++/Multimedia/Packet.hpp>
#include <Std++/Multimedia/Stream.hpp>
#include <Std++/Multimedia/SubtitleStream.hpp>
#include <Std++/Multimedia/VideoDecoder.hpp>
#include <Std++/Multimedia/VideoFrame.hpp>
#include <Std++/Multimedia/VideoStream.hpp>
#include <Std++/Multimedia/CAudioVoice.h>
#include <Std++/Multimedia/SMetaInfo.h>
//Pixmaps
//Pixmaps/YCbCr
#include <Std++/Multimedia/Pixmaps/YCbCr/YCbCr420Image.hpp>

//Multitasking
#include <Std++/Multitasking/ConditionVariable.hpp>
#include <Std++/Multitasking/Mutex.hpp>
#include <Std++/Multitasking/CProcess.h>
#include <Std++/Multitasking/StaticThreadPool.hpp>
#include <Std++/Multitasking/Thread.hpp>
#include <Std++/Multitasking/Multitasking.hpp>

//Network
#include <Std++/Network/DatagramSocket.hpp>
#include <Std++/Network/IPv4Address.hpp>

//PDF
#include <Std++/PDF/CDocument.h>

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
#include <Std++/Eventhandling/EventQueue.hpp>
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

//XML
#include <Std++/XML/Document.hpp>
#include <Std++/XML/TextNode.hpp>

//.
#include <Std++/__InitAndShutdown.h>
#include <Std++/Char.hpp>
#include <Std++/Function.hpp>
#include <Std++/ConfigurationFile.hpp>
#include <Std++/Tuple.hpp>
#include <Std++/Float.hpp>
#include <Std++/Integer.hpp>
#include <Std++/Mathematics.hpp>
#include <Std++/Optional.hpp>
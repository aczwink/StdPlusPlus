/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */

//General
#include <ACStdLib/__InitAndShutdown.h>
#include <ACStdLib/CFunction.h>
#include <ACStdLib/CIniFile.h>
#include <ACStdLib/CTuple.h>
#include <ACStdLib/Float.h>
#include <ACStdLib/Integer.h>
#include <ACStdLib/Mathematics.h>

//Compute
#include <ACStdLib/Compute/Device.hpp>

//Containers
#include <ACStdLib/Containers/AContainer.h>
//CFiniteSet
#include <ACStdLib/Containers/CFiniteSet/CFiniteSet.h>
//CLinkedList
#include <ACStdLib/Containers/CLinkedList/CLinkedList.h>
//CMap
#include <ACStdLib/Containers/CMap/CMap.h>
//Strings
#include <ACStdLib/Containers/Strings/C8BitString.h>
#include <ACStdLib/Containers/Strings/CString.h>
#include <ACStdLib/Containers/Strings/UTF-16/CUTF16String.h>

//Mathematics
#include <ACStdLib/Math/CDegree.h>
#include <ACStdLib/Math/CRadian.h>
#include <ACStdLib/Math/CSize.h>
//Mathematics/Geometry
#include <ACStdLib/Math/Geometry/CMatrix4x4.h>
#include <ACStdLib/Math/Geometry/CVector2.h>
#include <ACStdLib/Math/Geometry/CVector3.h>
#include <ACStdLib/Math/Geometry/CVector4.h>

//Streams
#include <ACStdLib/Streams/AOutputStream.h>
#include <ACStdLib/Streams/CBufferedInputStream.h>
#include <ACStdLib/Streams/CBufferInputStream.h>
#include <ACStdLib/Streams/CBufferOutputStream.h>
#include <ACStdLib/Streams/CConcatenatedInputStream.h>
#include <ACStdLib/Streams/CFileInputStream.h>
#include <ACStdLib/Streams/CFileOutputStream.h>
#include <ACStdLib/Streams/CStdOut.h>

//Bitstreams
#include <ACStdLib/Streams/Bitstreams/CBitInputStream.h>
#include <ACStdLib/Streams/Bitstreams/CBitInputStreamBitReversed.h>

//Readers
#include <ACStdLib/Streams/Readers/CEndianReader.h>
#include <ACStdLib/Streams/Readers/CTextReader.h>
//Streams/Writers
#include <ACStdLib/Streams/Writers/CTextWriter.h>

//CodeAnalysis
#include <ACStdLib/CodeAnalysis/CProgram.h>

//Compression
#include <ACStdLib/Compression/CHuffmanDecoder.h>
#include <ACStdLib/Compression/ADecompressor.h>

//Debugging
#include <ACStdLib/Debugging/CHook.h>

//ErrorHandling
#include <ACStdLib/ErrorHandling/AException.h>

//Filesystem
#include <ACStdLib/Filesystem/CPath.h>
#include <ACStdLib/Filesystem/CPathIterator.h>

//Formal languages
#include <ACStdLib/Formal Languages/CRegEx.h>
#include <ACStdLib/Formal Languages/CRegExParser.h>

//Internationalization
#include <ACStdLib/Internationalization/ALocale.h>

//Math
#include <ACStdLib/Math/CComplex.h>
#include <ACStdLib/Math/CFraction.h>
#include <ACStdLib/Math/CMatrix.h>
//Math/Geometry
#include <ACStdLib/Math/Geometry/AxisAlignedBox.h>
#include <ACStdLib/Math/Geometry/CMatrix3x3.h>
#include <ACStdLib/Math/Geometry/ObjectIntersection.h>

//Multimedia
#include <ACStdLib/Multimedia/AAudioDecoder.h>
#include <ACStdLib/Multimedia/ADemuxer.h>
#include <ACStdLib/Multimedia/AMuxer.h>
#include <ACStdLib/Multimedia/AStream.h>
#include <ACStdLib/Multimedia/AVideoDecoder.h>
#include <ACStdLib/Multimedia/CAudioBuffer.h>
#include <ACStdLib/Multimedia/CAudioFrame.h>
#include <ACStdLib/Multimedia/CAudioStream.h>
#include <ACStdLib/Multimedia/CAudioVoice.h>
#include <ACStdLib/Multimedia/CClusterIndex.h>
#include <ACStdLib/Multimedia/CSubtitleStream.h>
#include <ACStdLib/Multimedia/CVideoFrame.h>
#include <ACStdLib/Multimedia/CVideoStream.h>
#include <ACStdLib/Multimedia/ICodec.h>
#include <ACStdLib/Multimedia/IFormat.h>
#include <ACStdLib/Multimedia/SMetaInfo.h>
#include <ACStdLib/Multimedia/SPacket.h>
//Images
//Images/RGB
#include <ACStdLib/Multimedia/Images/RGB/CR8G8B8Image.h>
//Images/RGBA
#include <ACStdLib/Multimedia/Images/RGBA/CR8G8B8A8Image.h>
//Images/YCbCr
#include <ACStdLib/Multimedia/Images/YCbCr/CYCbCr420Image.h>

//Multitasking
#include <ACStdLib/Multitasking/CConditionVariable.h>
#include <ACStdLib/Multitasking/CMutex.h>
#include <ACStdLib/Multitasking/CProcess.h>
#include <ACStdLib/Multitasking/CStaticThreadPool.h>
#include <ACStdLib/Multitasking/CThread.h>
#include <ACStdLib/Multitasking/Multitasking.h>

//PDF
#include <ACStdLib/PDF/CDocument.h>

//Rendering
#include <ACStdLib/Rendering/CDeviceContext.h>
#include <ACStdLib/Rendering/CInputLayout.h>
#include <ACStdLib/Rendering/ICubeMap.h>
#include <ACStdLib/Rendering/IFrameBuffer.h>
#include <ACStdLib/Rendering/IIndexBuffer.h>
#include <ACStdLib/Rendering/IInputState.h>
#include <ACStdLib/Rendering/IShader.h>
#include <ACStdLib/Rendering/IShaderProgram.h>
#include <ACStdLib/Rendering/ITexture2D.h>
#include <ACStdLib/Rendering/IVertexBuffer.h>
//Rendering/2D
#include <ACStdLib/Rendering/2D/C2DPathRenderer.h>

//Time
#include <ACStdLib/Time/CCalendar.h>
#include <ACStdLib/Time/CTime.h>
#include <ACStdLib/Time/CTimer.h>
#include <ACStdLib/Time/Time.h>

//UI
#include <ACStdLib/UI/AWidgetContainer.h>
#include <ACStdLib/UI/CAction.hpp>
#include <ACStdLib/UI/CommonDialogs.h>
#include <ACStdLib/UI/CTerminalEventQueue.h>
#include <ACStdLib/UI/EventQueue.h>
#include <ACStdLib/UI/Keyboard.h>
#include <ACStdLib/UI/MainAppWindow.h>
#include <ACStdLib/UI/Mouse.h>
//Containers
#include <ACStdLib/UI/Containers/CTabBox.h>
#include <ACStdLib/UI/Containers/GroupBox.h>
//Controllers
#include <ACStdLib/UI/Controllers/ACheckListController.h>
#include <ACStdLib/UI/Controllers/AListController.h>
#include <ACStdLib/UI/Controllers/ATreeController.h>
//Controls
#include <ACStdLib/UI/Controls/CCheckBox.h>
#include <ACStdLib/UI/Controls/CDropDown.h>
#include <ACStdLib/UI/Controls/CLabel.h>
#include <ACStdLib/UI/Controls/CLineEdit.h>
#include <ACStdLib/UI/Controls/CProgressBar.h>
#include <ACStdLib/UI/Controls/CSlider.h>
#include <ACStdLib/UI/Controls/CTextEdit.h>
#include <ACStdLib/UI/Controls/PushButton.h>
//Events
#include <ACStdLib/UI/Events/CKeyEvent.h>
//Layouts
#include <ACStdLib/UI/Layouts/CGridLayout.h>
//Menu
#include <ACStdLib/UI/Menu/CMenu.h>
#include <ACStdLib/UI/Menu/CMenuBar.h>
//Transfer
#include <ACStdLib/UI/Transfer/CFileTransfer.h>
//Typing
#include <ACStdLib/UI/Typing/ITypeface.h>
//Views
#include <ACStdLib/UI/Views/C2DView.h>
#include <ACStdLib/UI/Views/C3DView.h>
#include <ACStdLib/UI/Views/CCheckListView.h>
#include <ACStdLib/UI/Views/CListView.h>
#include <ACStdLib/UI/Views/CTableView.h>
#include <ACStdLib/UI/Views/CTreeView.h>

//XML
#include <ACStdLib/XML/CDocument.h>
#include <ACStdLib/XML/CTextNode.h>
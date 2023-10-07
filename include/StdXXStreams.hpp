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

//Streams
#include <Std++/Streams/BlockInputStream.hpp>
#include <Std++/Streams/OutputStream.hpp>
#include <Std++/Streams/OutputStream.hpp>
#include <Std++/Streams/BufferedInputStream.hpp>
#include <Std++/Streams/BufferedOutputStream.hpp>
#include <Std++/Streams/BufferInputStream.hpp>
#include <Std++/Streams/BufferOutputStream.hpp>
#include <Std++/Streams/ConcatenatedInputStream.hpp>
#include <Std++/Streams/ChainedInputStream.hpp>
#include <Std++/Streams/FileInputStream.hpp>
#include <Std++/Streams/FileOutputStream.hpp>
#include <Std++/Cryptography/HashingInputStream.hpp>
#include <Std++/Cryptography/HashingOutputStream.hpp>
#include <Std++/Streams/LimitedInputStream.hpp>
#include <Std++/Streams/NullOutputStream.hpp>
#include <Std++/Streams/Writers/StdOut.hpp>
#include <Std++/Streams/StringInputStream.hpp>
#include <Std++/Streams/StdIn.hpp>
//Streams/Bitstreams
#include <Std++/Streams/Bitstreams/BitInputStream.hpp>
#include <Std++/Streams/Bitstreams/BitInputStreamBitReversed.hpp>
//Streams/Checksums
#include <Std++/Streams/Checksum/ChecksumInputStream.hpp>
//Streams/Readers
#include <Std++/Streams/Readers/CharStreamReader.hpp>
#include <Std++/Streams/Readers/DataReader.hpp>
#include <Std++/Streams/Readers/TextReader.hpp>
//Streams/Writers
#include <Std++/Streams/Writers/DataWriter.hpp>
#include <Std++/Streams/Writers/TextWriter.hpp>
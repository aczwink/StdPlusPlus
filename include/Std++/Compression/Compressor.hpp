/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
#pragma once
//Local
#include <Std++/Streams/OutputStream.hpp>
#include <Std++/Optional.hpp>
#include "CompressionAlgorithm.hpp"

namespace StdXX
{
	class STDPLUSPLUS_API Compressor : public OutputStream
	{
	public:
		//Constructor
		inline Compressor(OutputStream& outputStream) : outputStream(outputStream)
		{
		}

		//Destructor
		virtual ~Compressor()
		{
		}

		//Functions
		/**
		 *
		 * @param algorithm
		 * @param outputStream
		 * @param compressionLevel - A value in range 0 (least compression, fastest) and 9 (best compression, slowest). Don't specify to keep default value (dependent on algorithm)
		 * @return
		 */
		static Compressor *Create(CompressionAlgorithm algorithm, OutputStream& outputStream, Optional<uint8> compressionLevel = {});

	protected:
		//Members
		OutputStream& outputStream;
	};
}
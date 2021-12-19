/*
* Copyright (c) 2017-2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Strings/String.hpp>
#include <Std++/Containers/PriorityQueue.hpp>
#include <Std++/Tuple.hpp>
#include "CodingFormatId.hpp"

namespace StdXX
{
	namespace Multimedia
	{
		//Forward declarations
		class Decoder;
		class Encoder;
		class Parser;

		class STDPLUSPLUS_API CodingFormat
		{
		public:
			//Destructor
			virtual ~CodingFormat();

			//Abstract
			virtual CodingFormatId GetId() const = 0;
			virtual String GetName() const = 0;

			//Methods
			void AddDecoder(Decoder *decoder, float32 quality);
			void AddEncoder(Encoder *encoder, float32 quality);
			void AddParser(Parser *parser, float32 quality);
			const Decoder *GetBestMatchingDecoder() const;
			const Encoder *GetBestMatchingEncoder() const;
			const Parser *GetBestMatchingParser() const;

		private:
			//Members
			PriorityQueue<Tuple<float32, Decoder *>> decoders;
			PriorityQueue<Tuple<float32, Encoder *>> encoders;
			PriorityQueue<Tuple<float32, Parser *>> parsers;
		};
	}
}
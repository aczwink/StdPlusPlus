/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "Std++/Streams/InputStream.hpp"
#include "Element.hpp"
#include "Writer.hpp"

namespace StdXX::CommonFileFormats::XML
{
	class STDPLUSPLUS_API Document
	{
	public:
		//Constructor
		Document();

		//Destructor
		~Document();

		//Properties
		inline Element &Root()
		{
			return *this->rootElement;
		}

		//Inline
		inline void SetRootElement(Element *pElement)
		{
			delete this->rootElement;

			this->rootElement = pElement;
		}

		inline void Write(OutputStream& outputStream) const
		{
			Writer writer(outputStream);
			if(this->rootElement)
				this->rootElement->Write(writer);
		}

		//Functions
		static UniquePointer<Document> Parse(InputStream& inputStream);

	private:
		//Members
		Element *rootElement;
	};
}
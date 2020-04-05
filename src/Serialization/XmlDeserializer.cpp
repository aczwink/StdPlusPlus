/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include <Std++/Serialization/XmlDeserializer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats::XML;
using namespace StdXX::Serialization;

//Private methods
Element *XmlDeserializer::FirstChildElementWithTagName(Element &element, const String &tagName)
{
	for(Node* node : element.Children())
	{
		if( (node->GetType() == NodeType::Element) )
		{
			Element* child = dynamic_cast<Element*>(node);
			if(child->Name() == tagName)
				return child;
		}
	}
	return nullptr;
}

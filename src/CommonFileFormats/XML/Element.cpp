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
//Class Header
#include <Std++/CommonFileFormats/XML/Element.hpp>
//Local
#include <Std++/CommonFileFormats/XML/TextNode.hpp>
#include <Std++/CommonFileFormats/XML/Writer.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::CommonFileFormats::XML;

//Destructor
Element::~Element()
{
	for (const Node *const &refpChild : this->children)
	{
		delete refpChild;
	}
}

//Public methods
NodeType Element::GetType() const
{
	return NodeType::Element;
}

void Element::Write(Writer& writer) const
{
	writer.BeginElement(this->name, this->attributes);
	for(const auto& child : this->children)
		child->Write(writer);
	writer.EndElement();
}
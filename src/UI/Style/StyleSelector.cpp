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
//Class header
#include <Std++/UI/Style/StyleSelector.hpp>
//Local
#include <Std++/UI/Widget.hpp>
#include <Std++/UI/WidgetContainer.hpp>
//Namespaces
using namespace StdXX::UI;

//Public methods
bool StyleSelector::Matches(const Widget & widget) const
{
	const StyleContext& styleContext = widget.StyleContext();
	return this->Matches(styleContext, *widget.GetParent());
}

bool StyleSelector::Matches(const StyleContext& context, const Widget& parent) const
{
	if (!this->Matches(context))
		return false;

	if (!this->other.IsNull())
	{
		switch (this->combinator)
		{
		case StyleCombinator::Descendant:
		{
			const Widget* current = &parent;
			while (current)
			{
				if (this->other->Matches(*current))
					return true;
				current = current->GetParent();
			}
			return false;
		}
		break;
		default:
			NOT_IMPLEMENTED_ERROR;
		}
	}

	return true;
}

//Private methods
bool StyleSelector::Matches(const StyleContext& context) const
{
	if (!context.IsOfType(this->type))
		return false;

	if (!context.HasPseudoClasses(this->pseudoClasses))
		return false;

	return true;
}
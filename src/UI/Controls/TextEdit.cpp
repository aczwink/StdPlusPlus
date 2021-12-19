/*
 * Copyright (c) 2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/UI/Controls/TextEdit.hpp>
//Local
#include <Std++/_Backends/UI/UIBackend.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::UI;

//Constructor
TextEdit::TextEdit() : textEditBackend(nullptr)
{
	this->sizingPolicy.SetHorizontalPolicy(SizingPolicy::Policy::Expanding);
	this->sizingPolicy.SetVerticalPolicy(SizingPolicy::Policy::Expanding);
}

//Private methods
void TextEdit::RealizeSelf()
{
	_stdxx_::TextEditBackend* textEditBackend = this->_GetUIBackend()->CreateTextEditBackend(*this);
	this->_SetBackend(textEditBackend);
}

//Event handlers
void TextEdit::OnRealized()
{
	Widget::OnRealized();
	this->textEditBackend->SetEditable(this->editable);
	this->textEditBackend->SetText(this->text);
}
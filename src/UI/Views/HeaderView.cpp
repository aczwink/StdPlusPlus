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
#include <Std++/UI/Views/HeaderView.hpp>
//Local
#include <Std++/_Backends/UI/UIBackend.hpp>
//Namespaces
using namespace StdXX::UI;

//Private methods
void HeaderView::RealizeSelf()
{
	_stdxx_::HeaderViewBackend* headerViewBackend = this->_GetUIBackend()->CreateHeaderViewBackend(*this);
	this->_SetBackend(headerViewBackend);
}
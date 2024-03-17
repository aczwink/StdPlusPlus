/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/UI/DialogBackend.hpp>
#include "Window.hpp"

namespace StdXX::UI
{
	class Dialog : public Window
	{
	public:
		//Constructor
		inline Dialog()
		{
			this->parent = nullptr;
		}

		//Properties
		inline Window* Parent()
		{
			return this->parent;
		}

		//Methods
		bool Run(Window* parent, Property<bool>& isValid);

	private:
		//State
		Window* parent;
		_stdxx_::DialogBackend* dialogBackend;

		//Methods
		void RealizeSelf() override;
	};
}
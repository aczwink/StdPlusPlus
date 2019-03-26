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
#include <Std++/_Backends/UI/HeaderViewBackend.hpp>
#include "View.hpp"

namespace StdXX
{
	namespace UI
	{
		class HeaderView : public View
		{
		public:
			//Constructor
			inline HeaderView()
			{
				this->sizingPolicy.SetHorizontalPolicy(SizingPolicy::Policy::Expanding);
				this->sizingPolicy.SetVerticalPolicy(SizingPolicy::Policy::Fixed);
			}

			//Inline
			inline Math::RectD GetItemRect(uint32 number) const
			{
				return this->headerViewBackend->GetItemRect(number);
			}

		private:
			//Members
			_stdxx_::HeaderViewBackend* headerViewBackend;

			//Methods
			void RealizeSelf() override;

			//Inline
			inline void _SetBackend(_stdxx_::HeaderViewBackend* headerViewBackend)
			{
				View::_SetBackend(headerViewBackend);
				this->headerViewBackend = headerViewBackend;
			}
		};
	}
}
/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include "../Widget.hpp"

namespace StdPlusPlus
{
	namespace UI
	{
		class STDPLUSPLUS_API SpinBox : public Widget
		{
		public:
			//Constructor
			SpinBox(WidgetContainer *parent);

			//Inline
			inline void GetRange(int32 &min, int32 &max) const
			{
				this->backend->GetRange(min, max);
			}

			inline int32 GetValue() const
			{
				return this->backend->GetValue();
			}

			inline void SetRange(int32 min, int32 max) const
			{
				this->backend->SetRange(min, max);
			}
			
			inline void SetValue(int32 value)
			{
				this->backend->SetValue(value);
			}
		};
	}
}
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
#include <Std++/_Backends/UI/PushButtonBackend.hpp>
#include "../Widget.hpp"

namespace StdXX
{
    namespace UI
    {
        class STDPLUSPLUS_API PushButton : public Widget
        {
        public:
			//Dynamic event handlers
			Function<void()> onActivatedHandler;

            //Constructor
            PushButton();

            //Inline
            inline void SetText(const String &text)
			{
				this->text = text;
				if(this->pushButtonBackend)
					this->pushButtonBackend->SetText(this->text);
			}

		private:
        	//Members
			String text;
        	_stdxx_::PushButtonBackend *pushButtonBackend;

			//Methods
			void OnRealized() override;

			//Inline
			inline void _SetBackend(_stdxx_::PushButtonBackend* pushButtonBackend)
			{
				Widget::_SetBackend(pushButtonBackend);
				this->pushButtonBackend = pushButtonBackend;
			}
        };
    }
}
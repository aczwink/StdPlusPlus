/*
 * Copyright (c) 2017-2018,2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/UI/CheckBoxBackend.hpp>
#include "../Widget.hpp"

namespace StdXX
{
    namespace UI
    {
        class STDPLUSPLUS_API CheckBox : public Widget
        {
        public:
        	//Members
        	Signal<> toggled;

            //Constructor
            CheckBox();

            //Properties
			inline bool Checked() const
			{
				return this->isChecked;
			}

			inline void Checked(bool isChecked)
			{
				this->isChecked = isChecked;
				if(this->checkBoxBackend)
					this->checkBoxBackend->UpdateCheckState();
			}

			inline void Text(const String &text)
			{
				this->text = text;
				if(this->checkBoxBackend)
					this->checkBoxBackend->SetText(text);
			}

			//Methods
			void Event(UI::Event& e) override;

		private:
			//Members
        	bool isChecked;
			String text;
			_stdxx_::CheckBoxBackend *checkBoxBackend;

			//Event handlers
			virtual void OnRealized() override;

			//Methods
			void RealizeSelf() override;

			//Inline
			inline void _SetBackend(_stdxx_::CheckBoxBackend* checkBoxBackend)
			{
				Widget::_SetBackend(checkBoxBackend);
				this->checkBoxBackend = checkBoxBackend;
			}
        };
    }
}
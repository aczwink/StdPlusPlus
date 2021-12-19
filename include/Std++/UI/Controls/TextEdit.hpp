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
#include <Std++/UI/Widget.hpp>
#include <Std++/_Backends/UI/TextEditBackend.hpp>

namespace StdXX::UI
{
	class STDPLUSPLUS_API TextEdit : public Widget
	{
	public:
		//Constructor
		TextEdit();

		//Properties
		inline void Editable(bool editable)
		{
			this->editable = editable;
			if(this->textEditBackend)
				this->textEditBackend->SetEditable(editable);
		}

		inline void Text(const String &text)
		{
			this->text = text;
			if(this->textEditBackend)
			    this->textEditBackend->SetText(text);
		}

	protected:
		//Event handlers
		void OnRealized() override;

	private:
	    //Members
	    bool editable;
	    String text;
        _stdxx_::TextEditBackend* textEditBackend;

		//Methods
		void RealizeSelf() override;

        //Inline
        inline void _SetBackend(_stdxx_::TextEditBackend* textEditBackend)
        {
            Widget::_SetBackend(textEditBackend);
            this->textEditBackend = textEditBackend;
        }
	};
}
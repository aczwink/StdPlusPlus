/*
 * Copyright (c) 2017-2023 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/LinkedList/LinkedList.hpp>
#include "IndirectObject.hpp"
#include "StreamObject.hpp"
#include "Graphics.hpp"

namespace StdXX::CommonFileFormats::PDF
{
	//Forward declarations
	class Font;
	class Graphics;
	class PageTree;

    class Page : public IndirectObject
    {
		friend class Graphics;
    public:
    	//Constructor
    	Page(PageTree& parent);

		//Properties
		inline Graphics ContentGraphics()
		{
			return Graphics(this->content.buffer, *this);
		}

        //Methods
        void Visit(DocumentTreeVisitor &refVisitor);
        void Write(OutputStream &outputStream) override;

    private:
        //State
		PageTree& parent;
		float32 width;
		float32 height;
		LinkedList<Font *> usedFonts;
        StreamObject content;

		//Inline
		inline uint32 AddFont(Font *pFont)
		{
			int32 result;

			result = this->usedFonts.Find(pFont);
			if(result == -1)
			{
				this->usedFonts.InsertTail(pFont);
				result = this->usedFonts.GetNumberOfElements()-1;
			}

			return result;
		}
    };
}
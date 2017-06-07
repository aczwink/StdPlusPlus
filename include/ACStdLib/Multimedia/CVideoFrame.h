/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "Frame.hpp"
#include "AImage.h"

namespace ACStdLib
{
    namespace Multimedia
    {
        class ACSTDLIB_API CVideoFrame : public Frame
        {
        private:
            //Members
            AImage *pImage;

        public:
            //Constructor
            CVideoFrame(AImage *pImage);

            //Destructor
            ~CVideoFrame();

            //Methods
            DataType GetType() const;

            //Inline
            inline AImage *GetImage()
            {
                return this->pImage;
            }

            inline void SetImage(AImage *pImage)
            {
                this->pImage = pImage;
            }
        };
    }
}
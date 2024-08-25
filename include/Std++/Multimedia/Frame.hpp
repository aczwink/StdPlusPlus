/*
 * Copyright (c) 2017-2024 Amir Czwink (amir130@hotmail.de)
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
#include "../Definitions.h"
#include "../Unsigned.hpp"
#include "EnumTypes.hpp"
#include "AudioBuffer.hpp"
#include "Pixmap.hpp"

namespace StdXX
{
    namespace Multimedia
    {
        class Frame
        {
        public:
            //Members
            uint64 pts;

            //Constructors
            inline Frame(AudioBuffer* audioBuffer)
            {
                this->pts = Unsigned<uint64>::Max();
                this->dataType = DataType::Audio;
                this->audioBuffer = audioBuffer;
                this->pImage = nullptr;
            }

            inline Frame(Pixmap* pImage)
            {
                this->pts = Unsigned<uint64>::Max();
                this->dataType = DataType::Video;
                this->audioBuffer = nullptr;
                this->pImage = pImage;
            }

            //Destructor
            inline ~Frame()
            {
                delete this->audioBuffer;
                delete this->pImage;
            }

            //Inline
            inline const AudioBuffer* GetAudioBuffer() const
            {
                ASSERT_EQUALS(DataType::Audio, this->dataType);
                return this->audioBuffer;
            }

            //Inline
            inline Pixmap *GetImage()
            {
                ASSERT_EQUALS(DataType::Video, this->dataType);
                return this->pImage;
            }

            inline const Pixmap *GetPixmap() const
            {
                ASSERT_EQUALS(DataType::Video, this->dataType);
                return this->pImage;
            }

            inline DataType GetType() const
            {
                return this->dataType;
            }

            inline void SetImage(Pixmap *pImage)
            {
                ASSERT_EQUALS(DataType::Video, this->dataType);
                this->pImage = pImage;
            }

        private:
            //Members
            DataType dataType;
            AudioBuffer* audioBuffer;
            Pixmap *pImage;
        };
    }
}
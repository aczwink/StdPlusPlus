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
#include <Std++/_Backends/UI/ProgressBarBackend.hpp>
#include "../Widget.hpp"

namespace StdXX
{
    namespace UI
    {
        class ProgressBar : public Widget
        {
        public:
            //Constructor
            ProgressBar();

            //Properties
            inline void Progress(float64 progress)
            {
                this->progress = progress;
                if(this->progressBarBackend)
                    this->progressBarBackend->SetProgress(progress);
            }

        protected:
            //Event handlers
            void OnRealized() override;

        private:
            //Members
            _stdxx_::ProgressBarBackend* progressBarBackend;
            float64 progress;

            //Methods
            void RealizeSelf() override;

            //Inline
            inline void _SetBackend(_stdxx_::ProgressBarBackend* progressBarBackend)
            {
                Widget::_SetBackend(progressBarBackend);
                this->progressBarBackend = progressBarBackend;
            }
        };
    }
}
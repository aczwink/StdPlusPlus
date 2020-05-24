/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include "WidgetFrameBufferSetup.hpp"

namespace StdXX
{
    //Forward declarations
    namespace Rendering
    {
        class DeviceContext;
    }

    namespace UI
    {
        class STDPLUSPLUS_API RenderTargetWidget : public Widget
        {
        public:
            //Constructor
            RenderTargetWidget(const WidgetFrameBufferSetup& frameBufferSetup);

            //Destructor
            ~RenderTargetWidget();

            //Properties
            inline const Rendering::DeviceContext& DeviceContext() const
			{
            	return *this->deviceContext;
			}

            inline const WidgetFrameBufferSetup& FrameBufferSetup() const
            {
            	return this->frameBufferSetup;
            }

        protected:
            //Members
            Rendering::DeviceContext *deviceContext;

            //Eventhandlers
			virtual void OnRealized();
            virtual void OnResized() override;

        private:
        	//Members
        	WidgetFrameBufferSetup frameBufferSetup;

			//Methods
			void RealizeSelf() override;

            //Eventhandlers
            virtual void OnPaint(PaintEvent& event) override;
        };
    }
}
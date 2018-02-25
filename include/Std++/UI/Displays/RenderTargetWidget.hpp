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
#include "../WidgetContainer.hpp"

namespace StdPlusPlus
{
    //Forward declarations
    namespace Rendering
    {
        class DeviceContext;
    }

    namespace UI
    {
        class STDPLUSPLUS_API RenderTargetWidget : public WidgetContainer
        {
        private:
            //Eventhandlers
            virtual void OnPaint();

            //Methods
            void CreateOSHandle();
            void System_Destroy();

        protected:
            //Members
            Rendering::DeviceContext *deviceContext;

            //Eventhandlers
            virtual void OnResized();

        public:
            //Constructor
            RenderTargetWidget(WidgetContainer *pParent);

            //Destructor
            ~RenderTargetWidget();
        };
    }
}
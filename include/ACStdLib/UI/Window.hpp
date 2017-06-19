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
#include "ACStdLib/Containers/Strings/UTF-8/UTF8String.hpp"
#include "ACStdLib/Function.hpp"
#include "WidgetContainer.hpp"

namespace ACStdLib
{
    namespace UI
    {
        //Forward declarations
        class CDropTarget;
        class CMenuBar;
        class ITransfer;

        enum class EDropType
        {
            None, //reject drop
            Copy,
            Move,
            Link
        };

        class ACSTDLIB_API Window : public WidgetContainer
        {
            friend class CDropTarget;
            friend class EventQueue;
        private:
            //Members
            CMenuBar *pMenuBar;
            CDropTarget *pOSDropTarget;

            //Eventhandlers
            virtual void OnClose();
            virtual EDropType OnDragEnter(const ITransfer &refTransfer);
            virtual void OnDragLeave();
            virtual EDropType OnDragMove();
            virtual void OnDrop(const ITransfer &refTransfer);
            virtual void OnPaint();

            //Methods
            void CreateOSWindow(const Rect &refRect);
            void DestroyOSWindow();
            void MenuBarChangeOS();
            void Init(const Rect &refRect);

        protected:
            //Eventhandlers
            Function<void()> onDestroyEventHandler;

        public:
            //Constructors
            Window(ERenderMode mode = ERenderMode::OS);
            Window(const Rect &refRect, ERenderMode mode = ERenderMode::OS);
            Window(uint16 width, uint16 height, ERenderMode mode = ERenderMode::OS);

            //Destructor
            ~Window();

            //Methods
            void EnableDrop();
            void Maximize();
            void SetMenuBar(CMenuBar *pMenuBar);
            void SetTitle(const String &title);
            void ShowErrorBox(const String &title, const String &message);
            void ShowInformationBox(const String &refTitle, const String &refMessage);
            void SwitchFullscreen(bool state);

            //Inline
            inline CMenuBar *GetMenuBar()
            {
                return this->pMenuBar;
            }
        };
    }
}
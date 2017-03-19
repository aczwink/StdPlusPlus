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
#include "../Containers/Strings/UTF-8/CUTF8String.h"
#include "../CFunction.h"
#include "AWidgetContainer.h"

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

        class ACSTDLIB_API CWindow : public AWidgetContainer
        {
            friend class CDropTarget;
            friend class CEventQueue;
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
            void CreateOSWindow(const CRect &refRect);
            void DestroyOSWindow();
            void Init(const CRect &refRect);

        protected:
            //Eventhandlers
            CFunction<void()> onDestroyEventHandler;

        public:
            //Constructors
            CWindow(ERenderMode mode = ERenderMode::OS);
            CWindow(const CRect &refRect, ERenderMode mode = ERenderMode::OS);
            CWindow(uint16 width, uint16 height, ERenderMode mode = ERenderMode::OS);

            //Destructor
            ~CWindow();

            //Methods
            void EnableDrop();
            void Maximize();
            void SetMenuBar(CMenuBar *pMenuBar);
            void SetTitle(const CUTF8String &refTitle);
            void ShowErrorBox(const CString &refTitle, const CString &refMessage);
            void ShowInformationBox(const CString &refTitle, const CString &refMessage);
            void SwitchFullscreen(bool state);

            //Inline
            inline CMenuBar *GetMenuBar()
            {
                return this->pMenuBar;
            }
        };
    }
}
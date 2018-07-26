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
#include <Std++/UI/Containers/ContentAreaWidget.hpp>
#include <Std++/_Backends/UI/WindowBackend.hpp>
#include "../Containers/Strings/UTF-8/UTF8String.hpp"
#include "../Function.hpp"
#include "Menu/MenuBar.hpp"


namespace StdXX
{
    namespace UI
    {
        //Forward declarations
        class CDropTarget;
        class ITransfer;

        enum class EDropType
        {
            None, //reject drop
            Copy,
            Move,
            Link
        };

        class STDPLUSPLUS_API Window : public ContentAreaWidget
        {
            friend class CDropTarget;
            friend class UIEventSource;
        public:
            //Constructors
            Window();

            //Destructor
            ~Window();

            //Methods
            void EnableDrop();
            void ShowErrorBox(const OldString &title, const OldString &message);
            void SwitchFullscreen(bool state);

            //Inline
            inline MenuBar *GetMenuBar()
            {
                return this->pMenuBar;
            }

			inline void Maximize()
			{
				this->windowBackend->Maximize();
			}

			inline Path SelectExistingDirectory(const String &title, const Function<bool(const Path &)> &callback = [](const Path &){return true;}) const
			{
				return this->backend->SelectExistingDirectory(title, callback);
			}

			inline void SetMenuBar(MenuBar *menuBar)
			{
				delete this->pMenuBar;
				this->pMenuBar = menuBar;
				this->backend->SetMenuBar(this->pMenuBar, this->pMenuBar->backend);
			}

			inline void SetTitle(const String &title)
			{
				this->windowBackend->SetTitle(title);
			}

			inline void ShowInformationBox(const String &title, const String &message)
			{
				this->backend->ShowInformationBox(title, message);
			}

        private:
            //Members
        	_stdxx_::WindowBackend *windowBackend;
            MenuBar *pMenuBar;
            CDropTarget *pOSDropTarget;

            //Eventhandlers
            virtual void OnClose();
            virtual EDropType OnDragEnter(const ITransfer &refTransfer);
            virtual void OnDragLeave();
            virtual EDropType OnDragMove();
            virtual void OnDrop(const ITransfer &refTransfer);
        };
    }
}
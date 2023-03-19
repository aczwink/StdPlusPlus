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
#include <Std++/UI/Containers/ContentAreaWidget.hpp>
#include <Std++/_Backends/UI/WindowBackend.hpp>
#include <Std++/Tuple.hpp>
#include <Std++/UI/Menu/MenuBar.hpp>

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
            void SwitchFullscreen(bool state);

			//Overrideable
			virtual void Event(UI::Event& e) override;

            //Inline
            inline MenuBar *GetMenuBar()
            {
                return this->menuBar;
            }

			inline void Maximize()
			{
            	this->EnsureRealized();
				this->windowBackend->Maximize();
			}

			inline FileSystem::Path SelectExistingDirectory(const String &title, const Function<bool(const FileSystem::Path &)> &callback = [](const FileSystem::Path &){return true;}) const
			{
				return this->windowBackend->SelectExistingDirectory(title, callback);
			}

			/**
			 *
			 * @param title
			 * @param filters - Tuple of human-readable string and list of file extensions (without dot e.g. pdf). If this list is empty, all files are accepted.
			 * @param initialDirectory
			 * @return
			 */
			inline FileSystem::Path SelectExistingFile(const String& title, const DynamicArray<Tuple<String, DynamicArray<String>>>& filters, const FileSystem::Path& initialDirectory) const
			{
				return this->windowBackend->SelectExistingFile(title, filters, initialDirectory);
			}

			inline void SetMenuBar(MenuBar *menuBar)
			{
				delete this->menuBar;
				this->menuBar = menuBar;
				if(this->windowBackend)
					this->windowBackend->SetMenuBar(this->menuBar, this->menuBar->backend);
			}

			inline void SetTitle(const String &title)
			{
				this->title = title;
				if(this->windowBackend)
					this->windowBackend->SetTitle(title);
			}

			inline void ShowErrorBox(const String &title, const String &message)
			{
				this->windowBackend->ShowErrorBox(title, message);
			}

			inline void ShowInformationBox(const String &title, const String &message)
			{
				this->windowBackend->ShowInformationBox(title, message);
			}

		protected:
			//Eventhandlers
			virtual void OnRealized() override;

			//Inline
			inline void _SetBackend(_stdxx_::WindowBackend *windowBackend)
			{
				ContentAreaWidget::_SetBackend(windowBackend);
				this->windowBackend = windowBackend;
			}

        private:
            //Members
			/**
			 * To control that the window should realize after the initialization from the constructor.
			 */
			bool canRealize;
        	_stdxx_::WindowBackend *windowBackend;
            MenuBar* menuBar;
            CDropTarget *pOSDropTarget;
			String title;

			//Methods
			bool CanRealize() const override;
			void RealizeSelf() override;

            //Eventhandlers
            virtual void OnClose();
            virtual EDropType OnDragEnter(const ITransfer &refTransfer);
            virtual void OnDragLeave();
            virtual EDropType OnDragMove();
            virtual void OnDrop(const ITransfer &refTransfer);
        };
	}
}
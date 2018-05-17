/*
 * Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#ifdef _STDPLUSPLUS_BACKEND_COMMCTRL
//Global
#include <Windows.h>
#include <CommCtrl.h>
//Local
#include <Std++/_Backends/UIBackend.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "UI/CommCtrlWindowBackend.hpp"
#include "UI/Definitions.h"
#include "UI/WindowsMessageQueueEventSource.hpp"

//Manifest definition. Without it, InitCommonControls will fail and Visual Styles won't work
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace StdPlusPlus
{
	class CommCtrlBackend : public UIBackend
	{
	public:
		//Constructor
		CommCtrlBackend()
		{
			this->eventSource = new WindowsMessageQueueEventSource;
		}

		//Methods
		_stdpp::WindowBackend *CreateWindowBackend(_stdpp::WindowBackendType type, UI::Widget *widget)
		{
		    return new _stdpp::CommCtrlWindowBackend(this, type, widget);
		}

		EventSource *GetEventSource() const override
		{
			return this->eventSource.operator->();
		}

		void Load() override
		{
            //init control library
            INITCOMMONCONTROLSEX iccex;

            iccex.dwSize = sizeof(iccex);
            iccex.dwICC = ICC_BAR_CLASSES | ICC_LINK_CLASS | ICC_LISTVIEW_CLASSES | ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES;

            ASSERT(InitCommonControlsEx(&iccex), u8"Could not initialize common controls library.");

            //register window class
            WNDCLASSEXW wcex = {0};
            //wcex.style = CS_OWNDC;
            wcex.cbSize = sizeof(wcex);
            wcex.hInstance = GetModuleHandle(NULL);;
            wcex.lpfnWndProc = WindowsMessageQueueEventSource::WndProc;
            wcex.lpszClassName = STDPLUSPLUS_WIN_WNDCLASS;

            ASSERT(RegisterClassExW(&wcex), u8"Could not register window class");
		}

		void Unload() const override
		{
            if(!UnregisterClassW(STDPLUSPLUS_WIN_WNDCLASS, GetModuleHandle(NULL)))
            {
                DWORD lastErr;

                lastErr = GetLastError();
                switch(lastErr)
                {
                    case ERROR_CLASS_DOES_NOT_EXIST:
                    {
                        NOT_IMPLEMENTED_ERROR; //for some weird reason the class is sometimes not registered... i think when the program crashes..
                        //this should be checked
                    }
                        break;
                    case ERROR_CLASS_HAS_WINDOWS:
                    {
                        ASSERT(false, "You didn't close all windows");
                    }
                        break;
                    default:
                        NOT_IMPLEMENTED_ERROR;
                }
            }
		}

	private:
		//Members
		mutable UniquePointer<EventSource> eventSource;
	};
}
#endif
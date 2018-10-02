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
//Local
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "Imports.h"
#include "UI/CommCtrlMenuBackend.hpp"
#include "UI/CommCtrlMenuBarBackend.hpp"

//Manifest definition. Without it, InitCommonControls will fail and Visual Styles won't work
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace StdXX
{
	class CommCtrlBackend : public UIBackend
	{
	public:
		//Constructor
		CommCtrlBackend();

		//Methods
		_stdxx_::MenuBackend *CreateMenuBackend(UI::Menu *menu) override
		{
			NOT_IMPLEMENTED_ERROR; //TODO: reimplement me
			//return new _stdxx_::CommCtrlMenuBackend(menu);
			return nullptr;
		}

		_stdxx_::CheckBoxBackend * CreateCheckBoxBackend(UI::CheckBox * checkBox) override;
		EventSource *CreateEventSource() override;
		_stdxx_::GroupBoxBackend * CreateGroupBoxBackend(UI::GroupBox * groupBox) override;
		_stdxx_::LabelBackend * CreateLabelBackend(UI::Label * label) override;
		_stdxx_::MenuBarBackend *CreateMenuBarBackend(UI::MenuBar *menuBar) override;
		_stdxx_::PushButtonBackend * CreatePushButtonBackend(UI::PushButton * pushButton) override;
		_stdxx_::WidgetBackend * CreateRenderTargetWidgetBackend(UI::RenderTargetWidget * renderTargetWidget) override;
		_stdxx_::ContentAreaWidgetBackend* CreateScrollAreaBackend(UI::ScrollArea * scrollArea) override;
		_stdxx_::ViewBackend * CreateSelectBoxBackend(UI::SelectBox & selectBox) override;
		_stdxx_::SliderBackend * CreateSliderBackend(UI::Slider * slider) override;
		_stdxx_::SpinBoxBackend * CreateSpinBoxBackend(UI::SpinBox * spinBox) override;
		_stdxx_::WindowBackend * CreateWindowBackend(UI::Window * window) override;

		void Load() override;

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
	};
}
#endif
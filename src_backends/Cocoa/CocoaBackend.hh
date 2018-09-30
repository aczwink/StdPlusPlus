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
#ifdef _STDPLUSPLUS_BACKEND_COCOA
//Local
#include <Std++/_Backends/UI/UIBackend.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#import "CocoaOpenGL3CoreBackend.hh"

namespace StdXX
{
	class CocoaBackend : public UIBackend
	{
	public:
		//Constructor
		CocoaBackend()
		{
#ifdef _STDPLUSPLUS_BACKEND_OPENGL
			CocoaOpenGL3CoreBackend *cocoaOpenGL3CoreBackend = new CocoaOpenGL3CoreBackend;
			this->renderBackends.RegisterBackend(cocoaOpenGL3CoreBackend, 0);
#endif
		}

		//Methods
		_stdxx_::CheckBoxBackend *CreateCheckBoxBackend(UI::CheckBox *checkBox) override;
		EventSource *CreateEventSource() override;
		_stdxx_::GroupBoxBackend *CreateGroupBoxBackend(UI::GroupBox *groupBox) override;
		_stdxx_::LabelBackend *CreateLabelBackend(UI::Label *label) override;
		_stdxx_::MenuBackend *CreateMenuBackend(UI::Menu *menu) override;
		_stdxx_::MenuBarBackend *CreateMenuBarBackend(UI::MenuBar *menuBar) override;
		_stdxx_::PushButtonBackend *CreatePushButtonBackend(UI::PushButton *pushButton) override;
		_stdxx_::WidgetBackend *CreateRenderTargetWidgetBackend(UI::RenderTargetWidget *renderTargetWidget) override;
		_stdxx_::ContentAreaWidgetBackend *CreateScrollAreaBackend(UI::ScrollArea *scrollArea) override;
		_stdxx_::SliderBackend *CreateSliderBackend(UI::Slider *slider) override;
		_stdxx_::SpinBoxBackend *CreateSpinBoxBackend(UI::SpinBox *spinBox) override;
		_stdxx_::WindowBackend *CreateWindowBackend(UI::Window *window) override;
		void Load() override;
		void Unload() const override;
	};
}
#endif
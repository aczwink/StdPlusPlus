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
#include <Std++/_Backends/UIBackend.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#import "CocoaOpenGL3CoreBackend.hh"

namespace StdPlusPlus
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
		_stdpp::MenuBackend *CreateMenuBackend(UI::Menu *menu);
		_stdpp::MenuBarBackend *CreateMenuBarBackend(UI::MenuBar *menuBar);
		_stdpp::WindowBackend *CreateWindowBackend(_stdpp::WindowBackendType type, UI::Widget *widget);
		EventSource *GetEventSource() override;
		void Load() override;
		void Unload() const override;

	private:
		//Members
		EventSource *eventSource;
	};
}
#endif
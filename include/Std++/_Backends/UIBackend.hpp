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
#include <Std++/Eventhandling/EventSource.hpp>
#include "Backend.hpp"
#include "BackendManager.hpp"
#include "RenderBackend.hpp"
#include "WindowBackendType.hpp"

//Forward declarations
namespace _stdpp
{
	class MenuBackend;
	class MenuBarBackend;
	class WindowBackend;
}

namespace StdPlusPlus
{
	//Forward declarations
	class EventQueue;
	namespace UI
	{
		class Menu;
		class MenuBar;
		class Widget;
	}

	class UIBackend : public Backend
	{
	public:
		//Members
		BackendManager<RenderBackend> renderBackends;

		//Abstract
		virtual _stdpp::MenuBackend *CreateMenuBackend(UI::Menu *menu) = 0;
		virtual _stdpp::MenuBarBackend *CreateMenuBarBackend(UI::MenuBar *menuBar) = 0;
		virtual _stdpp::WindowBackend *CreateWindowBackend(_stdpp::WindowBackendType type, UI::Widget *widget) = 0;
		virtual EventSource *GetEventSource() = 0;

		//Methods
		BackendType GetType() const
		{
			return BackendType::UI;
		}
	};
}
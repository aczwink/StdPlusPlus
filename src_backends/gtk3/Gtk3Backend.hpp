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
#ifdef _STDPLUSPLUS_BACKEND_GTK3
//Global
#include <gtk/gtk.h>
//Local
#include <Std++/_Backends/UIBackend.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "UI/GtkWindowBackend.hpp"
#include "Gtk3OpenGLBackend.hpp"
#include "UI/GtkEventSource.hpp"

namespace StdPlusPlus
{
	class Gtk3Backend : public UIBackend
	{
	public:
		//Constructor
		Gtk3Backend()
		{
			this->eventSource = new GtkEventSource;

			//register render backends
#ifdef _STDPLUSPLUS_BACKEND_OPENGL
			Gtk3OpenGLBackend *gtkOpenGLBackend = new Gtk3OpenGLBackend;
			this->renderBackends.RegisterBackend(gtkOpenGLBackend, 0);
#endif
		}

		//Methods
		_stdpp::WindowBackend *CreateWindowBackend(_stdpp::WindowBackendType type, Widget *widget)
		{
			return new _stdpp::GtkWindowBackend(this, type, widget, nullptr);
		}

		EventSource *GetEventSource() override
		{
			return this->eventSource.operator->();
		}

		void Load()
		{
			gtk_init(nullptr, nullptr);
		}

	private:
		//Members
		UniquePointer<GtkEventSource> eventSource;
	};
}
#endif
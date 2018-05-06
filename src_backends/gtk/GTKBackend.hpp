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
#include "GTKOpenGLBackend.hpp"
#include "UI/GtkEventSource.hpp"

namespace StdPlusPlus
{
	class GTKBackend : public UIBackend
	{
	public:
		//Constructor
		GTKBackend()
		{
			this->eventSource = new GtkEventSource;

			//register render backends
#ifdef _STDPLUSPLUS_BACKEND_OPENGL
			GTKOpenGLBackend *gtkOpenGLBackend = new GTKOpenGLBackend;
			this->renderBackends.RegisterBackend(gtkOpenGLBackend, 0);
#endif
		}

		//Methods
		_stdpp::WindowBackend *CreateWindowBackend(_stdpp::WindowBackendType type, Widget *widget)
		{
			return new _stdpp::GtkWindowBackend(this, type, widget, nullptr);
		}

		EventSource *GetEventSource() const override
		{
			this->eventSource.operator->();
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
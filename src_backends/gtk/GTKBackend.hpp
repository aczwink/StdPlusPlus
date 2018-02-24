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
#include "UI/GtkWindowBackend.hpp"

namespace StdPlusPlus
{
	class GTKBackend : public UIBackend
	{
	public:
		//Methods
		_stdpp::WindowBackend *CreateWindowBackend(_stdpp::WindowBackendType type, Widget *widget) const
		{
			return new _stdpp::GtkWindowBackend(type, widget);
		}

		void Load() const
		{
			gtk_init(0, nullptr);
		}
	};
}
#endif
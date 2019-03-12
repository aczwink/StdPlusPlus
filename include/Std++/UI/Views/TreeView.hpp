/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/_Backends/UI/DrawableWidgetBackend.hpp>
#include "HeaderView.hpp"
#include "View.hpp"

//Forward declarations
namespace _stdxx_
{
	class TreeViewBody;
}

namespace StdXX
{
    namespace UI
    {
        //Forward declarations
        class TreeController;

        class TreeView : public View
        {
            friend class TreeController;
            friend class EventQueue;
        public:
            //Constructor
            TreeView();

		private:
			//Members
			HeaderView* headerView;
			_stdxx_::TreeViewBody* body;

			//Methods
			void RealizeSelf() override;

			//Event handlers
			void OnModelChanged();
        };
    }
}
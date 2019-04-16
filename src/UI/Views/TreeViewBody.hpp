/*
 * Copyright (c) 2019 Amir Czwink (amir130@hotmail.de)
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
//Local
#include <Std++/SmartPointers/SharedPointer.hpp>
#include <Std++/UI/Controllers/TreeController.hpp>
#include <Std++/UI/Views/HeaderView.hpp>
#include <Std++/UI/DrawableWidget.hpp>

namespace _stdxx_
{
	class Cell
	{
	public:
		//Members
		StdXX::UI::ControllerIndex controllerIndex;
	};

	class Row
	{
	public:
		//Members
		bool isExpanded;
		float64 height;
		StdXX::UI::ControllerIndex controllerIndex;
		StdXX::DynamicArray<Cell> items;
		uint32 nChildren;
		Row* parent;
		Row* firstChild;
		Row* next;

		//Constructor
		inline Row()
		{
			this->isExpanded = false;
			this->height = 0;
			this->nChildren = 0;
			this->parent = nullptr;
			this->firstChild = nullptr;
			this->next = nullptr;
		}
	};

	class TreeViewBody : public StdXX::UI::DrawableWidget
	{
	public:
		//Constructor
		TreeViewBody(const StdXX::UI::HeaderView& headerView);

		//Destructor
		~TreeViewBody();

		//Inline
		inline void SetController(StdXX::SharedPointer<StdXX::UI::TreeController> controller)
		{
			this->controller = controller;
		}

	private:
		//Members
		Row* root;
		Row* top;
		StdXX::SharedPointer<StdXX::UI::TreeController> controller;
		const StdXX::UI::HeaderView& headerView;

		//Methods
		void DrawCell(StdXX::UI::Painter& painter, Cell& cell, const StdXX::Math::RectD& rect, bool isFirstCol);
		void DrawRow(StdXX::UI::Painter& painter, Row& row, float64 top);
		Row* NextVisible(Row* row) const;
		void RequireCellsBuffered(Row& row) const;
		void RequireHeight(Row& row, StdXX::UI::Painter& painter) const;

		//Event handlers
		void OnPaint(StdXX::UI::PaintEvent& event) override;
		void OnResized() override;
	};
}
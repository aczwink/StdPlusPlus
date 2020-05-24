/*
 * Copyright (c) 2017-2020 Amir Czwink (amir130@hotmail.de)
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
#include "../../Containers/Array/DynamicArray.hpp"
#include "../../Containers/LinkedList/LinkedList.hpp"
#include <Std++/Math/Vector2/Vector2.inl>

namespace StdXX::Rendering
{
	class FlatVectorPath
	{
		friend class VectorPath;
	public:
		struct Point
		{
			Math::Vector2D pos;
			Math::Vector2D dir;
			float64 length;
			Math::Vector2D extrusion;
			bool isCorner;
			bool bevelInner;
			bool bevelOuter;
		};

	private:
		//Members
		LinkedList<Point> points;
		bool isClosed;
		bool isConvex;

	public:
		//Constructor
		inline FlatVectorPath()
		{
			this->isClosed = false;
			this->isConvex = false;
		}

		//Methods
		void ComputeJoins(float64 w, float64 miterLimit);
		void ComputePointAttributes();

		//Inline
		inline void AddCorner(const Math::Vector2D &refV)
		{
			this->AddPoint(refV, true);
		}

		inline void AddPoint(const Math::Vector2D &refV, bool isCorner)
		{
			this->points.InsertTail({refV, Math::Vector2D(), 0, Math::Vector2D(), isCorner, false, false});
		}

		inline const Point &GetPoint(uint32 index) const
		{
			return this->points[index];
		}

		inline const LinkedList<Point> &GetPoints() const
		{
			return this->points;
		}

		inline bool IsClosed() const
		{
			return this->isClosed;
		}

		inline bool IsConvex() const
		{
			return this->isConvex;
		}
	};
}
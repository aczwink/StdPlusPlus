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
//Class header
#include "Win32Painter.hpp"
//Local
#include <Std++/Unsigned.hpp>
//Namespaces
using namespace _stdxx_;
using namespace StdXX;

//Destructor
Win32Painter::~Win32Painter()
{
	DeleteObject(this->currentBrush);
	EndPaint(this->hWnd, &this->ps);
}

//Public methods
void Win32Painter::BeginPath()
{
	::BeginPath(this->hDC);
}

void _stdxx_::Win32Painter::BezierTo(const Math::Vector2D & refCP0, const Math::Vector2D & refCP1, const Math::Vector2D & refTo)
{
	NOT_IMPLEMENTED_ERROR;
}

void Win32Painter::ClosePath()
{
	EndPath(this->hDC);
}

void Win32Painter::Fill()
{
	FillPath(this->hDC);
}

void Win32Painter::LineTo(const Math::Vector2D & v)
{
	::LineTo(this->hDC, v.x, this->yInverter - v.y);
}

void Win32Painter::MoveTo(const Math::Vector2D & v)
{
	MoveToEx(this->hDC, v.x, this->yInverter - v.y, nullptr);
}

void Win32Painter::SetFillColor(const Color & color)
{
	DeleteObject(this->currentBrush);
	this->currentBrush = CreateSolidBrush(this->MapColor(color));
	SelectObject(this->hDC, this->currentBrush);
}

void _stdxx_::Win32Painter::SetStrokeColor(const Color & refStrokeColor)
{
	NOT_IMPLEMENTED_ERROR;
}

void _stdxx_::Win32Painter::SetStrokeWidth(float64 strokeWidth)
{
	NOT_IMPLEMENTED_ERROR;
}

void _stdxx_::Win32Painter::Stroke()
{
	NOT_IMPLEMENTED_ERROR;
}

//Private methods
COLORREF Win32Painter::MapColor(const StdXX::Color& color) const
{
	uint8 r = Math::Clamp(uint8(color.r * Unsigned<uint8>::Max()), 0u8, Unsigned<uint8>::Max());
	uint8 g = Math::Clamp(uint8(color.g * Unsigned<uint8>::Max()), 0u8, Unsigned<uint8>::Max());
	uint8 b = Math::Clamp(uint8(color.b * Unsigned<uint8>::Max()), 0u8, Unsigned<uint8>::Max());

	return RGB(r, g, b);
}
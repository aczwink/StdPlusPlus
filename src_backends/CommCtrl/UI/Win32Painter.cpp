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

//Constructor
Win32Painter::Win32Painter(Win32Window& window)
{
	this->hWnd = window.GetHWND();
	this->yInverter = window.GetClientRect().height();
	this->hDC = BeginPaint(this->hWnd, &this->ps);
	this->currentBrush = nullptr;
	this->isPenUpToDate = true;
	this->currentPen = nullptr;
	this->currentPenColor = 0;
	this->currentPenWidth = 1;

	SelectObject(this->hDC, GetStockObject(DEFAULT_GUI_FONT));
	TEXTMETRICW tm;
	GetTextMetricsW(this->hDC, &tm);
	this->textHeight = tm.tmHeight;
}

//Destructor
Win32Painter::~Win32Painter()
{
	DeleteObject(this->currentBrush);
	DeleteObject(this->currentPen);
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

Math::SizeD Win32Painter::ComputeTextSize(const String & text) const
{
	SIZE size;
	GetTextExtentPoint32W(this->hDC, (LPCWSTR)text.ToUTF16().GetRawData(), text.ToUTF16().GetLength(), &size);

	return { (float64)size.cx, (float64)size.cy };
}

void Win32Painter::DrawText(const Math::PointD & p, const String & text)
{
	SetBkMode(this->hDC, TRANSPARENT);
	SetTextColor(this->hDC, this->currentFillColor);
	TextOutW(this->hDC, p.x, this->yInverter - p.y - this->textHeight, (LPCWSTR)text.ToUTF16().GetRawData(), text.ToUTF16().GetSize() / sizeof(uint16));
	SetBkMode(this->hDC, OPAQUE);
}

void Win32Painter::Fill()
{
	this->MakeSureBrushIsUpToDate();
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
	this->currentFillColor = this->MapColor(color);
	this->isBrushUpToDate = false;
}

void Win32Painter::SetStrokeColor(const Color & color)
{
	this->currentPenColor = this->MapColor(color);
	this->isPenUpToDate = false;
}

void Win32Painter::SetStrokeWidth(float64 strokeWidth)
{
	this->currentPenWidth = strokeWidth;
	this->isPenUpToDate = false;
}

void Win32Painter::Stroke()
{
	this->MakeSurePenIsUpToDate();
	StrokePath(this->hDC);
}

//Private methods
void Win32Painter::MakeSureBrushIsUpToDate()
{
	if (!this->isBrushUpToDate)
	{
		DeleteObject(this->currentBrush);
		this->currentBrush = CreateSolidBrush(this->currentFillColor);
		SelectObject(this->hDC, this->currentBrush);

		this->isBrushUpToDate = true;
	}
}

void Win32Painter::MakeSurePenIsUpToDate()
{
	if (!this->isPenUpToDate)
	{
		DeleteObject(this->currentPen);
		//this->currentPen = CreatePen(PS_SOLID, this->currentPenWidth, this->currentPenColor);
		LOGBRUSH brush;
		brush.lbStyle = BS_SOLID;
		brush.lbColor = this->currentPenColor;
		this->currentPen = ExtCreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, this->currentPenWidth, &brush, 0, nullptr);
		SelectObject(this->hDC, this->currentPen);
		this->isPenUpToDate = true;
	}
}

COLORREF Win32Painter::MapColor(const StdXX::Color& color) const
{
	uint8 r = Math::Clamp(uint8(color.r * Unsigned<uint8>::Max()), 0_u8, Unsigned<uint8>::Max());
	uint8 g = Math::Clamp(uint8(color.g * Unsigned<uint8>::Max()), 0_u8, Unsigned<uint8>::Max());
	uint8 b = Math::Clamp(uint8(color.b * Unsigned<uint8>::Max()), 0_u8, Unsigned<uint8>::Max());

	return RGB(r, g, b);
}
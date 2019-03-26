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
#include <Std++/UI/Painter.hpp>
#include "Win32Window.hpp"

namespace _stdxx_
{
	class Win32Painter : public StdXX::UI::Painter
	{
	public:
		//Constructor
		Win32Painter(Win32Window& window);

		//Destructor
		~Win32Painter();

		//Methods
		void BeginPath() override;
		void BezierTo(const StdXX::Math::Vector2D & refCP0, const StdXX::Math::Vector2D & refCP1, const StdXX::Math::Vector2D & refTo) override;
		void ClosePath() override;
		StdXX::Math::SizeD ComputeTextSize(const StdXX::String & text) const override;
		void DrawText(const StdXX::Math::PointD & p, const StdXX::String & text) override;
		void Fill() override;
		void LineTo(const StdXX::Math::Vector2D & refV) override;
		void MoveTo(const StdXX::Math::Vector2D & refV) override;
		void SetFillColor(const StdXX::Color & color) override;
		void SetStrokeColor(const StdXX::Color & refStrokeColor) override;
		void SetStrokeWidth(float64 strokeWidth) override;
		void Stroke() override;

	private:
		//Members
		HWND hWnd;
		HDC hDC;
		PAINTSTRUCT ps;
		bool isBrushUpToDate;
		COLORREF currentFillColor;
		HBRUSH currentBrush;
		bool isPenUpToDate;
		HPEN currentPen;
		COLORREF currentPenColor;
		uint32 currentPenWidth;
		float64 yInverter;
		uint32 textHeight;

		//Methods
		void MakeSureBrushIsUpToDate();
		void MakeSurePenIsUpToDate();
		COLORREF MapColor(const StdXX::Color& color) const;
	};
}
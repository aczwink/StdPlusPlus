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
#include "../DeviceContext.hpp"
#include "../InputState.hpp"
#include "../ShaderProgram.hpp"
#include "../VertexBuffer.hpp"
#include "HostComputedPathRenderer.hpp"

namespace StdXX::Rendering
{
	class STDPLUSPLUS_API DevicePathRenderer : public HostComputedPathRenderer
	{
		enum class RenderMethod
		{
			Fill,
			FillConvex,
			Stroke
		};

		struct RenderEntry
		{
			RenderMethod renderMethod;
			Color color;
			ITexture* texture;
			DynamicArray<PathAttributes> pathAttributes;
			uint32 boundingRectangleOffset;
			Math::Vector2D boundingRectMin;
			Math::Vector2D boundingRectMax;
		};

	public:
		//Constructor
		DevicePathRenderer(DeviceContext &refDC);

		//Destructor
		~DevicePathRenderer();

		//Methods from parents
		using VectorPathRenderer::BezierTo;
		using VectorPathRenderer::MoveTo;
		using VectorPathRenderer::LineTo;

		//Methods
		void Sync();

		//Inline
		inline void Scale(float32 scaleX, float32 scaleY)
		{
			this->state.transform = Math::Matrix3S::Scale({scaleX, scaleY, 0.0f}) * this->state.transform;
		}

		inline void SetFillColor(const Color &refFillColor)
		{
			this->state.fillColor = refFillColor;
		}

		inline void SetFillTexture(ITexture* texture)
		{
			this->texture = texture;
		}

		inline void SetStrokeColor(const Color &refStrokeColor)
		{
			this->state.strokeColor = refStrokeColor;
		}

		inline void SetStrokeWidth(float64 strokeWidth)
		{
			this->state.strokeWidth = strokeWidth;
		}

		template <typename ScalarType>
		inline void SetViewPort(const Math::Size<ScalarType> &size)
		{
			this->viewSize = Math::Vector2S(size.width, size.height);
		}

	private:
		//Members
		LinkedList<RenderEntry> renderCalls;
		ShaderProgram *shaderProgram;
		InputState *pInputState;
		VertexBuffer *pVertexBuffer;
		ITexture* texture;
		Math::Vector2S viewSize;
		struct
		{
			uint32 color;
			uint32 fragmentType;
			uint32 textureSampler;
			uint32 transform;
			uint32 uvTransform;
		} uniformLocations;

		DeviceContext &refDC;

		//Methods
		void InitRendering();
		void RenderFill(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<PathAttributes> &&pathAttributes, Math::Vector2D boundingRectMin, Math::Vector2D boundingRectMax);
		void RenderStroke(const DynamicArray<FlatVectorPath *> &refPaths, DynamicArray<PathAttributes> &&refPathAttributes);
	};
}
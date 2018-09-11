/*
* Copyright (c) 2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Compute/DeviceContext.hpp>
#include <Std++/Devices/ComputeDevice.hpp>
#include <Std++/SmartPointers/AutoPointer.hpp>
#include <Std++/SmartPointers/UniquePointer.hpp>
#include <Std++/Optional.hpp>
#include "Pixmap.hpp"

namespace StdXX
{
	namespace Multimedia
	{
		class STDPLUSPLUS_API ComputePixmapResampler
		{
		public:
			//Constructor
			ComputePixmapResampler(const Pixmap &pixmap, const PixelFormat &sourcePixelFormat);

			//Methods
			Pixmap *Run();

			//Inline
			inline void ChangePixelFormat(const PixelFormat &targetPixelFormat)
			{
				this->targetPixelFormat = targetPixelFormat;
			}

		private:
			//Members
			const Pixmap &pixmap;
			PixelFormat sourcePixelFormat;
			AutoPointer<Device> device;
			UniquePointer<Compute::DeviceContext> dc;
			UniquePointer<Compute::CommandQueue> commandQueue;
			Optional<PixelFormat> targetPixelFormat;
			UniquePointer<Compute::Program> program;

			//Methods
			void CompileProgram();
			String GenerateMainResampleFunctionCode(uint8 nPixelsPerWorker) const;
			String GenerateReadPixelFunctionCode(uint8 nPixelsPerWorker) const;
			String GenerateWritePixelsFunctionCode(uint8 nPixelsPerWorker) const;
			String GetPixelFormatConversionFunctionCode() const;
		};
	}
}
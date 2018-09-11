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
//Class header
#include <Std++/Multimedia/ComputePixmapResampler.hpp>
//Local
#include <Std++/Devices/DeviceEnumerator.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Compute;
using namespace StdXX::Multimedia;

//Constructor
ComputePixmapResampler::ComputePixmapResampler(const Pixmap &pixmap, const PixelFormat &sourcePixelFormat) : pixmap(pixmap), sourcePixelFormat(sourcePixelFormat)
{
	//get a device
	DeviceEnumerator enumerator(DeviceType::Compute);
	this->device = enumerator.GetNextDevice();

	//get device context
	this->dc = this->device.Cast<ComputeDevice>()->CreateDeviceContext();

	//create command queue
	this->commandQueue = this->dc->CreateCommandQueue();
}

#include <Std++/Streams/StdOut.hpp>

//Public methods
Pixmap *ComputePixmapResampler::Run()
{
	this->CompileProgram();

	//create kernel
	UniquePointer<Kernel> kernel = this->program->CreateKernel(u8"ResampleMain");

	//TODO: create program code
	NOT_IMPLEMENTED_ERROR; //TODO: implement me

	/*
	//create buffers
	UniquePointer<Buffer> srcBuf = this->dc->CreateBuffer(W * H * 3);
	UniquePointer<Buffer> dstBuf = this->dc->CreateBuffer(W * H * 3 * 4);

	//write input pixmap
	commandQueue->EnqueueWriteBuffer(*srcBuf, false, 0, W * H * 3, pixmap);

	//call kernel
	kernel->SetArg(0, *srcBuf);
	kernel->SetArg(1, *dstBuf);
	commandQueue->EnqueueTask(*kernel, W*H);

	//wait for kernel to finish
	commandQueue->Finish();

	//read back transformed pixmap
	commandQueue->EnqueueReadBuffer(*dstBuf, false, 0, W * H * 3 * 4, dst);
	commandQueue->Finish();

	*/
	NOT_IMPLEMENTED_ERROR; //this stays here until this method is solidly tested
	return nullptr;
}

//Private methods
void ComputePixmapResampler::CompileProgram()
{
	//determine the number of pixels per worker.
	uint8 nPixelsPerWorker = 0;
	for (uint8 i = 0; i < this->sourcePixelFormat.nPlanes; i++)
	{
		//we can only write multiples of 32bit (without byte-addressing-store extensions)
		uint8 horzSampleFactor;
		uint8 currentBlockSize = this->sourcePixelFormat.ComputeBlockSize(i, horzSampleFactor);
		4 / currentBlockSize * horzSampleFactor;
	}

	//add functions
	String programCode;
	if (this->targetPixelFormat.HasValue())
		programCode += this->GetPixelFormatConversionFunctionCode();
	programCode += this->GenerateReadPixelFunctionCode(nPixelsPerWorker);
	programCode += this->GenerateWritePixelsFunctionCode(nPixelsPerWorker);
	programCode += this->GenerateMainResampleFunctionCode(nPixelsPerWorker);

	stdOut << programCode << endl;

	//create program object
	this->program = this->dc->CreateProgram(programCode);
	bool result = this->program->Build();
	if (!result)
	{
		//should not happen... other than if we created wrong code 
		ASSERT(false, program->GetBuildLog());
	}
}

String ComputePixmapResampler::GenerateMainResampleFunctionCode(uint8 nPixelsPerWorker) const
{
	String programCode;

	//create kernel code
	programCode += u8"__kernel void ResampleMain(";
	for (uint8 i = 0; i < this->sourcePixelFormat.nPlanes; i++)
		programCode += u8"__global uchar* in" + String::Number(i) + u8", ";
	bool needComma = false;
	for (uint8 i = 0; i < this->targetPixelFormat->nPlanes; i++)
	{
		if(needComma)
			programCode += u8", ";
		programCode += u8"__global uchar* out" + String::Number(i);
		needComma = true;
	}

	programCode += u8")\r\n{\r\n";

	programCode += u8"uint y = get_global_id(0) * " + String::Number(nPixelsPerWorker) + u8" / " + String::Number(this->pixmap.GetSize().width) + u8";\r\n";
	programCode += u8"uint x = get_global_id(0) * " + String::Number(nPixelsPerWorker) + u8" % " + String::Number(this->pixmap.GetSize().width) + u8";\r\n";

	//first step: read pixels
	programCode += u8"float4 in_pixels[" + String::Number(nPixelsPerWorker) + u8"];\r\n";
	programCode += u8"for(uint i = 0; i < " + String::Number(nPixelsPerWorker) + u8"; i++)\r\n";
	programCode += u8"{\r\n";
	programCode += u8"uint y_cur = y + ((x + i) / " + String::Number(this->pixmap.GetSize().width) + u8");\r\n";
	programCode += u8"uint x_cur = (x + i) % " + String::Number(this->pixmap.GetSize().width) + u8";\r\n";
	programCode += u8"in_pixels[i] = ReadPixel(";
	for (uint8 i = 0; i < this->sourcePixelFormat.nPlanes; i++)
		programCode += u8"in" + String::Number(i) + u8", ";
	programCode += u8"y_cur, x_cur);\r\n";
	programCode += u8"}\r\n";

	//second step: convert pixels
	programCode += u8"float4 out_pixels[" + String::Number(nPixelsPerWorker) + u8"];\r\n";
	programCode += u8"for(uint i = 0; i < " + String::Number(nPixelsPerWorker) + u8"; i++)\r\n";
	programCode += u8"{\r\n";
	programCode += u8"out_pixels[i] = ChangePixelFormat(in_pixels[i]);\r\n";
	programCode += u8"}\r\n";

	//third step: write pixels
	programCode += u8"WritePixels(out_pixels, ";
	for (uint8 i = 0; i < this->targetPixelFormat->nPlanes; i++)
		programCode += u8"out" + String::Number(i) + u8", ";
	programCode += u8"y, x);\r\n";
	/*
	//first step: read input into a vec4 of floats in range[0, 1]
	programCode += u8"uint x, y, lineSize;\r\n";
	programCode += u8"float4 in_c;\r\n";
	for (uint8 i = 0; i < this->sourcePixelFormat.GetNumberOfColorComponents(); i++)
	{
		const auto &cc = this->sourcePixelFormat.colorComponents[i];

		ASSERT(cc.shift % 8 == 0, u8"TODO: IMPLEMENT NON BYTE INDEX");
		ASSERT(cc.nBits == 8, u8"TODO: IMPLEMENT OTHER THAN BYTE READING");

		//get x and y from global id
		programCode += u8"lineSize = " + String::Number(this->pixmap.GetLineSize(cc.planeIndex)) + u8";\r\n";
		programCode += u8"y = get_global_id(0) / lineSize;\r\n";
		programCode += u8"x = get_global_id(0) % lineSize;\r\n";

		for (uint8 j = 0; j < cc.vertSampleFactor; j++)
		{
			for (uint8 k = 0; k < cc.horzSampleFactor; k++)
			{
				//get correct pointer
				uint8 sampleFactor;
				uint8 blockSize = this->sourcePixelFormat.ComputeBlockSize(cc.planeIndex, sampleFactor);
				programCode += u8"uchar* in_c" + String::Number(i) + u8"_ptr" + String::Number(j) + u8"_" + String::Number(k) + u8" = in" + String::Number(cc.planeIndex)
					+ u8"[(y+" + String::Number(j) + u8")*lineSize+(x+" + String::Number(k) + u8")*" + String::Number(blockSize) + u8"/" + String::Number(sampleFactor) + u8"+"
					+ String::Number(cc.shift / 8) + u8"]; \r\n";
			}
		}

		//read and scale input		
		programCode += u8"in_c.s" + String::Number(i) + u8" = (((0";
		for (uint8 j = 0; j < cc.vertSampleFactor; j++)
		{
			for (uint8 k = 0; k < cc.horzSampleFactor; k++)
			{
				programCode += u8" + ((float)(*in_c" + String::Number(i) + u8"_ptr" + String::Number(j) + u8"_" + String::Number(k) + u8"))";
			}
		}
		programCode += u8") / " + String::Number(cc.vertSampleFactor * cc.horzSampleFactor) + u8") - " + String::Number(cc.min.u8) + u8".0f) / " + String::Number(cc.max.u8) + u8".0f;\r\n";
	}

	//second step: convert pixel format
	if (this->targetPixelFormat.HasValue())
		programCode += u8"float4 out_c = ChangePixelFormat(in_c);\r\n";

	//third step: write pixels
	*/

	programCode += u8"}";

	return programCode;
}

String ComputePixmapResampler::GenerateReadPixelFunctionCode(uint8 nPixelsPerWorker) const
{
	String programCode;

	//read input into a vec4 of floats in range[0, 1]
	programCode = u8"float4 ReadPixel(";
	for (uint8 i = 0; i < this->sourcePixelFormat.nPlanes; i++)
		programCode += u8"__global uchar* in" + String::Number(i) + u8", ";
	programCode += u8"uint y, uint x)\r\n{\r\n";
	
	//read input into a vec4 of floats in range[0, 1]
	programCode += u8"uint lineSize;\r\n";
	programCode += u8"float4 result;\r\n";
	for (uint8 i = 0; i < this->sourcePixelFormat.GetNumberOfColorComponents(); i++)
	{
		const auto &cc = this->sourcePixelFormat.colorComponents[i];

		ASSERT(cc.shift % 8 == 0, u8"TODO: IMPLEMENT NON BYTE INDEX");
		ASSERT(cc.nBits == 8, u8"TODO: IMPLEMENT OTHER THAN BYTE READING");

		//get x and y from global id
		programCode += u8"lineSize = " + String::Number(this->pixmap.GetLineSize(cc.planeIndex)) + u8";\r\n";
		
		//get correct pointer
		uint8 sampleFactor;
		uint8 blockSize = this->sourcePixelFormat.ComputeBlockSize(cc.planeIndex, sampleFactor);
		programCode += u8"__global uchar* in_c" + String::Number(i) + u8"_ptr = &in" + String::Number(cc.planeIndex)
			+ u8"[(y/" + String::Number(cc.vertSampleFactor) + u8")*lineSize+(x/" + String::Number(cc.horzSampleFactor) + u8")*" + String::Number(blockSize) + u8"+"
			+ String::Number(cc.shift / 8) + u8"]; \r\n";
		
		//read and scale input
		programCode += u8"result.s" + String::Number(i) + u8" = + (((float)(*in_c" + String::Number(i) + u8"_ptr)) - " + String::Number(cc.min.u8) + u8".0f) / " + String::Number(cc.max.u8) + u8".0f;\r\n";
	}

	programCode += u8"return result;\r\n";
	programCode += u8"}\r\n";

	return programCode;
}

String ComputePixmapResampler::GenerateWritePixelsFunctionCode(uint8 nPixelsPerWorker) const
{
	String programCode;

	programCode = u8"void WritePixels(float4 pixels[" + String::Number(nPixelsPerWorker) + u8"], ";
	for (uint8 i = 0; i < this->targetPixelFormat->nPlanes; i++)
		programCode += u8"__global uchar* out" + String::Number(i) + u8", ";
	programCode += u8"uint y, uint x)\r\n";
	programCode += u8"{\r\n";
	programCode += u8"}\r\n";

	return programCode;
}

String ComputePixmapResampler::GetPixelFormatConversionFunctionCode() const
{
	String functionCode;

	/*
	all pixel format conversions are always done in float [0, 1] range.
	We therefore assume here that input is scaled appropriatly.
	*/

	static_assert(PixelFormat::MAX_COLOR_COMPONENTS <= 4); //else we need to return something else then float4

	functionCode = u8"float4 ChangePixelFormat(const float4 in)\r\n{";
	switch (this->sourcePixelFormat.colorSpace)
	{
	case ColorSpace::YCbCr:
	{
		switch (this->targetPixelFormat->colorSpace)
		{
		case ColorSpace::RGB:
			//using BT.601 here... how to know which one is correct?!
			functionCode += u8R"(
const float K_R = 0.299f;
const float K_G = 0.587f;
const float K_B = 0.114f;

const float y = in.x;
const float cb = in.y;
const float cr = in.z;

float r = y + 0.5f * (1.0f - K_R) * cr;
float g = y - 0.5f * (1.0f - K_B) * (K_B / K_G) * cb - 0.5f * (1.0f - K_R) * (K_R / K_G) * cr;
float b = y + 0.5f * (1.0f - K_B) * cb;

return (float4)(r, g, b, 0);
			)";
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
		}
	}
	break;
	default:
		NOT_IMPLEMENTED_ERROR;
	}
	functionCode += u8"}\r\n";

	return functionCode;
}
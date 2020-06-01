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
#include <Std++/SmartPointers/UniquePointer.hpp>
#include "../Streams/SeekableInputStream.hpp"
#include "MediaObject.hpp"
#include "Packet.hpp"

namespace StdXX::Multimedia
{
	class Demuxer : public MediaObject
	{
	public:
		//Constructor
		inline Demuxer(const Format &refFormat, SeekableInputStream &refInput) : MediaObject(refFormat), inputStream(refInput)
		{
			this->bitRate = 0;
		}

		//Abstract
		virtual void ReadHeader() = 0;
		virtual void Seek(uint64 timestamp, const class TimeScale& timeScale) = 0;

		//Methods
		bool FindStreamInfo();
		UniquePointer<IPacket> ReadFrame();

		//Properties
		inline uint64 EndTime() const
		{
			return this->startTime + this->duration;
		}

		//Inline
		inline uint32 GetBitRate() const
		{
			return this->bitRate;
		}

		inline uint64 GetStartTime() const
		{
			return this->startTime;
		}

	protected:
		//Members
		SeekableInputStream & inputStream;

		//Overrideable
		/**
		 * Flushes the state that the demuxer has about the current input situation.
		 * This does not include stream or codec information but everything that it identifies with the current input stream position
		 * (cached packets that would arrive sequentially now etc.).
		 * After this operation, the demuxer should have no idea where he is in the stream.
		 * However, if the demuxer does not support byte-seeking, it can assume that it will be at a valid point in the stream (not in the middle of a data packet or so).
		 */
		virtual void Reset();

	private:
		//Members
		uint32 bitRate;

		//Abstract
		virtual UniquePointer<IPacket> ReadPacket() = 0;

		//Methods
		bool AllInfoIsAvailable();
		bool AllStreamsHaveDuration() const;
		void DeriveDurationFromPacketTimestamps();
		void ExtractInfo(const IPacket& packet);
		void InitializeDecoders();
		void InitializeParsers();
		bool TryToAllocateDecoder(Stream& stream);
	};
}
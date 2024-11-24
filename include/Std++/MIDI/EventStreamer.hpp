/*
 * Copyright (c) 2022-2024 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Array/FixedArray.hpp>
#include "Program.hpp"

namespace StdXX::MIDI
{
	enum class TrackType
	{
		ChannelTrack,
		MetaTrack,
	};

	struct NextEventResult
	{
		TrackType trackType;
		uint32 channelTrackNumber;
		uint32 trackIndex;
		uint64 deltaTime;
	};

	class EventStreamer
	{
	public:
		//Constructor
		inline EventStreamer(const Program& program) : program(program), channelIndices(program.NumberOfChannelTracks())
		{
			for(uint32& channelIndex : this->channelIndices)
				channelIndex = 0;
		}

		//Methods
		bool FindNextEvent(NextEventResult &result);

	private:
		//State
		const Program& program;
		uint32 metaTrackIdx = 0;
		FixedArray<uint32> channelIndices;
		uint64 t = 0;

		//Methods
		void Update(const NextEventResult& nextEventResult);
	};
}
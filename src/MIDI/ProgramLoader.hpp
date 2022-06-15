/*
 * Copyright (c) 2022 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/MIDI/Program.hpp>
#include <Std++/Streams/InputStream.hpp>
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>
#include "SMFEvent.hpp"

namespace _stdxx_
{
    class ProgramLoader
    {
    public:
        //Methods
        StdXX::MIDI::Program Load(StdXX::InputStream& inputStream) const;

    private:
        //Methods
        bool IsChannelMessage(SMFEventType eventType) const;
        void ReadTrack(StdXX::InputStream& inputStream, StdXX::MIDI::Program& program, StdXX::BinaryTreeMap<uint8, uint16>& channelMap, uint16 channelToTrackOffset) const;
    };
}
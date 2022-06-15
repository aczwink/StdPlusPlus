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
//Class header
#include <Std++/MIDI/Factory.hpp>
//Local
#include <Std++/_Backends/BackendManager.hpp>
#include <Std++/_Backends/MIDIBackend.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::MIDI;

//Constructor
Factory::Factory()
{
    this->backend = BackendManager<MIDIBackend>::GetRootInstance().GetActiveBackend();
}

//Public methods
UniquePointer<StandardOutputSequencer> Factory::ConnectToOutput(const String &portId) const
{
    return this->backend->ConnectToOutput(portId);
}

DynamicArray<Port> Factory::QueryPorts() const
{
    return this->backend->QueryPorts();
}
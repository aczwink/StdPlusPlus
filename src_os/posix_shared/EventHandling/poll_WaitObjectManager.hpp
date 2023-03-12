/*
 * Copyright (c) 2020-2021 Amir Czwink (amir130@hotmail.de)
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
//Global
#include <poll.h>
//Local
#include <Std++/EventHandling/WaitObjectManager.hpp>
#include <Std++/Containers/Array/DynamicArray.hpp>
#include <Std++/Math/Interval.hpp>
#include <Std++/Containers/BinaryTreeMap/BinaryTreeMap.hpp>

namespace _stdxx_
{
	class poll_WaitObjectManager : public StdXX::EventHandling::WaitObjectManager
	{
	public:
		//Methods
		void Add(const StdXX::EventHandling::EventSource& source, int fd, int16 nativeEvents) override;
		void AddWaitForInput(const StdXX::EventHandling::EventSource& source, const StdXX::OSHandle &osHandle) override;
		void AddWaitForInput(const StdXX::EventHandling::EventSource& source, int fd) override;
		void Clear() override;
		StdXX::UniquePointer<StdXX::EventHandling::WaitResult> FetchWaitResult(const StdXX::EventHandling::EventSource &eventSource) override;

		//Properties
		inline pollfd* PollFDs()
		{
			return &this->pollfds[0];
		}

		inline uint32 PollFDsCount() const
		{
			return this->pollfds.GetNumberOfElements();
		}

	private:
		//Members
		StdXX::DynamicArray<pollfd> pollfds;
		StdXX::BinaryTreeMap<const StdXX::EventHandling::EventSource*, StdXX::Math::Interval<uint32>> ranges;
	};
}
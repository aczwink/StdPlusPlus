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
#pragma once
//Local
#include <Std++/Containers/Container.hpp>
#include "HashTableEntry.hpp"

namespace StdXX
{
	template<typename T>
	struct HashFunction
	{
	};

	template <>
	struct HashFunction<uint32>
	{
		uint32 operator()(uint32 v) const
		{
			return v;
		}
	};

	/**
	 * Open hashing implementation.
	 *
	 * @tparam KeyType
	 * @tparam DataType
	 * @tparam HashType
	 */
	template<typename KeyType, typename ValueType, typename HashType = HashFunction<KeyType>>
	class HashMap : public Container
	{
		typedef _stdxx_::HashTableEntry<KeyType, ValueType> Entry;
	public:
		//Constructor
		inline HashMap() : buckets(nullptr), nBuckets(0)
		{
		}

		//Operators
		const ValueType &operator[](const KeyType& key) const
		{
			const Entry* entry = this->FindEntry(key);
			ASSERT(entry, "A non-existant key of const HashMap was accessed.");
			return entry->value;
		}

		//Methods
		void Insert(KeyType&& key, ValueType&& value)
		{
			if(this->nBuckets == 0)
				this->Reserve(16);

			HashType hasher;
			uint32 hash = hasher(key) % this->nBuckets;

			if(this->buckets[hash] == nullptr)
				this->buckets[hash] = new Entry(Move(key), Move(value));
			else
			{
				Entry* entry = this->buckets[hash];
				Entry* lastEntry = nullptr;
				while(entry)
				{
					if(entry->key == key)
					{
						entry->value = Move(value);
						return;
					}

					lastEntry = entry;
					entry = entry->next;
				}

				lastEntry->next = new Entry(Move(key), Move(value));
			}

			this->nElements++;
		}

		void Reserve(uint32 nBuckets)
		{
			if(nBuckets <= this->nBuckets)
				return;

			Entry** newBuckets = new Entry*[nBuckets];
			for(uint32 i = 0; i < nBuckets; i++)
				newBuckets[i] = nullptr;
			//TODO rehash all entries into newBuckets

			this->buckets = newBuckets;
			this->nBuckets = nBuckets;
		}

	private:
		//Members
		Entry** buckets;
		uint32 nBuckets;

		//Methods
		const Entry* FindEntry(const KeyType& key) const
		{
			HashType hasher;
			uint32 hash = hasher(key) % this->nBuckets;

			Entry* entry = this->buckets[hash];
			while(entry)
			{
				if(entry->key == key)
					return entry;
				entry = entry->next;
			}
			return nullptr;
		}
	};
}
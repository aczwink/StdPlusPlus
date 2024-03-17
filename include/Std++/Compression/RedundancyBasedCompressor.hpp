/*
 * Copyright (c) 2024 Amir Czwink (amir130@hotmail.de)
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
#include "Compressor.hpp"
#include "IndexedSlidingDictionary.hpp"

namespace StdXX
{
	class RedundancyBasedCompressor : public Compressor
	{
	public:
		struct Config
		{
			uint8 minBackrefLength;
			uint16 maxBackrefLength;
			uint16 maxDistance;
			uint8 initByte;
			uint16 initLength;
		};
	public:
		//Constructor
		RedundancyBasedCompressor(OutputStream &outputStream, Config config);

		//Methods
		void Finalize() override;
		void Flush() override;
		uint32 WriteBytes(const void *source, uint32 size) override;

	protected:
		//Inline
		inline void ReadSlidingWindowPart(void* destination, uint16 distance, uint16 length) const
		{
			this->dictionary.Read(destination, this->nUnprocessedBytesInDictionary + distance, length);
		}

	private:
		//State
		Config config;
		/**
		 * This dictionary is used to store the coming up bytes as well as the sliding dictionary.
		 * This way it serves as both the sliding window and a buffer.
		 * This is why it has to be the double size of the backreferences that can be found.
		 */
		StdXX::IndexedSlidingDictionary dictionary;
		uint32 nUnprocessedBytesInDictionary;
		/**
		 * The number of unprocessed bytes in the dictionary that is required for compression to start
		 */
		uint32 targetUnprocessedBytesCount;

		//Abstract
		/**
		 * @return The number of bytes that should be checked for overlapping longer backreferences.
		 * Return < 0 if you want to reject this backreference. In that case the next byte will be directly emitted.
		 */
		virtual int32 CheckPrimaryBackreference(uint16 distance, uint16 length) = 0;
		/**
		 * @return true if the second should be preferred over the first. Else false
		 */
		virtual bool CheckSecondaryBackreference(uint16 d1, uint16 l1, uint16 d2, uint16 l2, uint16 distanceBetweenBackreferences) = 0;
		virtual void EmitBackreference(uint16 distance, uint16 length) = 0;
		virtual void EmitDirectByte() = 0;

		//Methods
		void EmitBlock();
		bool TryFindBetterMatch(const StdXX::IndexedSlidingDictionary::DictionaryMatch &startMatch, uint16 length);

		//Inline
		inline void InitDictionary()
		{
			for(uint16 i = 0; i < this->config.initLength; i++)
				this->dictionary.Append(this->config.initByte);
			this->UpdateIndex();
		}

		inline void UpdateIndex()
		{
			this->dictionary.IndexUpTo(this->nUnprocessedBytesInDictionary);
		}
	};
}
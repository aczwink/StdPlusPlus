/*
 * Copyright (c) 2017-2021 Amir Czwink (amir130@hotmail.de)
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
#include <Std++/Containers/Array/DynamicArray.hpp>
#include <Std++/Containers/ResizeableSequenceContainer.hpp>
#include <Std++/Debug.hpp>
#include <Std++/Definitions.h>
#include <Std++/Unsigned.hpp>
#include <Std++/Utility.hpp>

namespace StdXX
{
	//Forward declarations
	class ConstStringIterator;

	enum class FloatDisplayMode
	{
		/**
		 * The more reasonable one, either fixed point or scientific.
		 * The precision specifies the maximum number of significant digits.
		 * Trailing zeroes including the decimal separator are omitted.
		 */
		Auto = u8'G',
		/**
		 * The number is formatted as [-]9.9
		 * The precision is the number of digits AFTER the decimal point
		 */
		FixedPointNotation = u8'f',
		/**
		 * The number is formatted as [-]9.9E[+|-]999, where 'E' stands for 'x 10^' i.e. multiplicated power of 10
		 * The precision is the number of digits AFTER the decimal point
		 */
		ScientificNotation = u8'E',
	};

	/**
	 * The standard string representation of Std++.
	 * Data is Unicode codepoints, encoded in either UTF-8 or UTF-16 (host byte-order) to efficiently store data.
	 * As these formats are variable length codings, this class does not provide the subscript-operator,
	 * because the operation has a complexity of O(n).
	 * When random access is required, consider using UTF32String instead, which is a fixed length coding and therefore provides constant-time access.
	 * However, in most cases sequential access is sufficient and UTF-8/UTF-16 encondings are more space-efficient than UTF-32.
	 *
	 * The class defaults to use UTF-16 except on linux where UTF-8 is used, as the whole linux kernel and file system use UTF-8.
	 *
	 * This class uses implicitly shared resources with CopyFrom-on-write policy.
	 * A null byte is appended to the end of the string only if an object has exclusive access to a resource.
	 */
	class String
	{
		friend class ConstStringIterator;

		class Resource : public ResizeableSequenceContainer<byte>
		{
			friend class String;
		public:
			//Members
			uint32 referenceCounter;
			bool isUTF8;

			//Constructors
			inline Resource() : referenceCounter(1)
			{
#ifdef XPC_OS_WINDOWS
                this->isUTF8 = false;
#else
                this->isUTF8 = true;
#endif
			}

			//Inline
			inline Resource *Copy(uint32 offset, uint32 size) const
			{
				Resource *copy = new Resource;

				copy->isUTF8 = this->isUTF8;
				copy->EnsureCapacity(size);
				MemCopy(copy->data, this->data + offset, size);
				copy->nElements = size;

				return copy;
			}

			inline void EnsureCapacity(uint32 requiredNumberOfElements)
			{
				//always include null char in capacity so that GetRawData()
				// does not cause a resize just because of the null byte
				requiredNumberOfElements += 2; //2 because in case of UTF-16 the zero-termination is two bytes long
				ResizeableSequenceContainer<byte>::EnsureCapacity(requiredNumberOfElements);
			}

			inline void Release()
			{
				if(--this->referenceCounter == 0)
					delete this;
			}
		};
	public:
		//Constructors
		inline String() : sharedResource(nullptr), data(nullptr), size(0), length(0)
		{
		}

		inline String(const String &other) : sharedResource(nullptr), data(nullptr) //copy ctor
		{
			*this = other;
		}

		inline String(String &&other) : sharedResource(nullptr), data(nullptr) //move ctor
		{
			*this = Move(other);
		}

		/**
		 * This constructor assumes that the parameter passed is UTF-8 encoded and zero terminated.
		 * (What else should it assume?!). Therefore never provide literals without the "u8" prefix.
		 * In addition, it assumes that the pointer is valid throughout the lifetime of the string object.
		 * It does NOT copy the string but reference it.
		 * For a copy instead use String::CopyRawString.
		 *
		 * @param utf8 An UTF-8 and zero terminated string.
		 */
		inline String(const char *utf8Literal) : sharedResource(nullptr), data(reinterpret_cast<const uint8 *>(utf8Literal))
		{
			this->length = this->CountUTF8Length(reinterpret_cast<const uint8 *>(utf8Literal), this->size);
		}

		/**
		 * This constructor assumes that the pointer is valid throughout the lifetime of the string object.
		 * It does NOT copy the string but reference it.
		 * For a copy instead use String::CopyRawString.
		 *
		 * @param utf8Literal A zero terminated string of chars encoded in UTF-8.
		 */
		inline String(const char8_t* utf8Literal) : sharedResource(nullptr),
			data(reinterpret_cast<const uint8 *>(utf8Literal))
		{
			this->length = this->CountUTF8Length(reinterpret_cast<const uint8 *>(utf8Literal), this->size);
		}

		//Destructor
		~String()
		{
			this->Release();
		}

		//Inline operators
		inline String &operator=(const String &rhs) //copy assign
		{
			this->Release();

			this->sharedResource = rhs.sharedResource;
			this->data = rhs.data;
			this->size = rhs.size;
			this->length = rhs.length;
			if(this->sharedResource)
				this->sharedResource->referenceCounter++;

			return *this;
		}

		inline String &operator=(String &&rhs) //move assign
		{
			this->Release();

			this->sharedResource = rhs.sharedResource;
			this->data = rhs.data;
			this->size = rhs.size;
			this->length = rhs.length;
			rhs.sharedResource = nullptr;
			rhs.data = nullptr;
			rhs.size = 0;
			rhs.length = 0;

			return *this;
		}

		//Operators
		String &operator+=(uint32 codePoint);
		String &operator+=(const String &rhs);
		bool operator==(const String &rhs) const;
		bool operator<(const String &rhs) const;

		//Inline operators
		inline String operator+(const String &rhs) const
		{
			String result(*this);
			return result += rhs;
		}

		inline bool operator>(const String &rhs) const
		{
			return rhs < *this;
		}

		inline bool operator!=(const String &rhs) const
		{
			return !(*this == rhs);
		}

		//Methods
		bool EndsWith(const String &string) const;
		/**
		 * Tries to find 'string' in this string within the range of 'startPos' and 'length' and on success returns the position of the first match.
		 * If 'string' can't be found, Natural<uint32>::Max() is returned.
		 * The result and the range parameters are character indices, not byte indices.
		 *
		 * @param string
		 * @param startPos
		 * @param endPos
		 * @return
		 */
		uint32 Find(const String &string, uint32 startPos = 0, uint32 length = Unsigned<uint32>::Max()) const;
		/**
		 * The same as Find but traverses the string in reversed order. The operation starts at 'startPos' (from the beginning of the string),
		 * and iterates at most 'length' characters backwards.
		 *
		 * @param string
		 * @param startPos
		 * @param length
		 * @return
		 */
		uint32 FindReverse(const String &string, uint32 startPos = Unsigned<uint32>::Max(), uint32 length = Unsigned<uint32>::Max()) const;
		uint64 ParseHexNumber() const;
		String Replace(const String &from, const String &to) const;
		String Reversed() const;
		DynamicArray<String> Split(const String &seperator) const;
		bool StartsWith(const String &string) const;
		/**
		 * Returns the slice defined by the range of 'startPos' and 'length' in character units.
		 * If the end of the slice is outside this string, the slice is trimmed to match the end of this string.
		 *
		 * @param startPos
		 * @param length
		 * @return
		 */
		String SubString(uint32 startPos, uint32 length = Unsigned<uint32>::Max()) const;
		int64 ToInt() const;
		String ToLowercase() const;
		uint64 ToUInt() const;
		String ToUppercase() const;
		/**
		 * Convert internal representation to UTF-8 if it not already is in this enconding.
		 *
		 * @return *this
		 */
		const String &ToUTF8() const;
		/**
		* Convert internal representation to UTF-16 if it not already is in this enconding.
		*
		* @return *this
		*/
		const String &ToUTF16() const;
		/**
		 * Returns this string but trimmed from whitespaces from the beginning and the end.
		 * @return
		 */
		String Trim() const;

		//Inline
		inline bool Contains(const String& subString) const
		{
			return this->Find(subString) != Unsigned<uint32>::Max();
		}

		inline uint32 GetLength() const
		{
			return this->length;
		}

		/**
		 * Return pointer to memory containing codepoints encoded according to this strings encoding.
		 * The memory is NOT zero terminated and therefore only the first 'size of this string' bytes that are valid.
		 * @return
		 */
		inline const byte *GetRawData() const
		{
			return this->data;
		}

		/**
		 * Return pointer to memory containing codepoints encoded according to this strings encoding.
		 * The result is zero terminated.
		 * The method can therefore cause the string to acquire an exclusive resource if it does not have already.
		 *
		 * @return Zero-terminated memory block of encoded codepoints
		 */
		inline const uint8 *GetRawZeroTerminatedData() const
		{
			this->Detach();
			if (this->sharedResource->isUTF8)
				this->sharedResource->data[this->sharedResource->nElements] = 0;
			else
				*((uint16 *)&this->sharedResource->data[this->sharedResource->nElements]) = 0;

			return this->data;
		}

		inline uint32 GetSize() const
		{
			return this->size;
		}

		inline bool IsEmpty() const
		{
			return this->length == 0;
		}

		inline void Release()
		{
			if(this->sharedResource)
			{
				this->sharedResource->Release();
				this->sharedResource = nullptr;
			}
		}

		inline int32 ToInt32() const
		{
			return (int32)this->ToInt();
		}

		inline uint32 ToUInt32() const
		{
			return static_cast<uint32>(this->ToUInt());
		}

		//Functions
		/**
		 * Assumes that the parameter is UTF-8 encoded and zero terminated.
		 * Does a deep-copy of the source string.
		 *
		 * @param utf8
		 * @return
		 */
		static String CopyRawString(const char *utf8);

		/**
		 * Assumes that the parameter is UTF-16 (host endian order) encoded;
		 * Further if nChars equals Unsigned<uint32>::Max(), then the function will assume the input to be zero-terminated.
		 * The function will in this case copy chars until it finds a null character.
		 * Otherwise it copies exactly nChars characters.
		 * Does a deep-copy of the source string.
		 *
		 */
		static String CopyRawString(const uint16 *utf16, uint32 nChars = Unsigned<uint32>::Max());

		/**
		 * Deep-copies size bytes of UTF-8 data into string.
		 * @param utf8
		 * @param size
		 * @return
		 */
		static String CopyUtf8Bytes(const uint8* utf8, uint32 size);

		/**
		 * Formats a value using binary prefixes.
		 * For instance the input 1024, 'B' is transformed to "1 KiB".
		 *
		 * @param
		 * @return
		 */
		static String FormatBinaryPrefixed(uint64 value, const String &suffix = u8"B");
		static String HexNumber(uint64 value, uint8 nMinChars = 0, bool addBase = true);
		/**
		 * Parses a number with base 10 and returns it as a string.
		 * The result is preceeded by length(unpadded result)-minLength zeros.
		 *
		 * @param value
		 * @return
		 */
		static String Number(uint64 value, uint8 base = 10, uint8 minLength = 0);
		static String Number(float64 number, FloatDisplayMode displayMode = FloatDisplayMode::Auto, uint8 precision = 6);

		//Inline functions
		template <typename Iterable>
		inline static String Join(const Iterable& iterable, const String& separator)
		{
			String joined;
			for(const String& string : iterable)
			{
				if(!joined.IsEmpty())
					joined += separator;
				joined += string;
			}
			return joined;
		}

		inline static String Number(uint16 natural)
		{
			return String::Number((uint64)natural);
		}

		inline static String Number(int32 natural)
		{
			return String::Number((int64)natural);
		}

		inline static String Number(uint32 natural)
		{
			return String::Number((uint64)natural);
		}

		inline static String Number(int64 value)
		{
			if(value < 0)
				return String(u8"-") + String::Number((uint64)(-value));
			return String::Number((uint64)value);
		}

		//For range-based loop
		ConstStringIterator begin() const;
		ConstStringIterator end() const;

	private:
		//Members
		mutable Resource *sharedResource;
		mutable const uint8 *data;
		mutable uint32 size;
		uint32 length;

		//Methods
		uint32 DecodeUTF8(const uint8 *src, uint8 &nBytes) const;
		uint32 DecodeUTF16(const uint16 *src, bool &isSurrogate) const;
		/**
		 * I.e. This wants to write the resource.
		 */
		void Detach() const;
		uint8 EncodeUTF8(uint32 codePoint, byte *dest) const;
		bool EncodeUTF16(uint32 codePoint, uint16 *dest) const;
		ConstStringIterator GetIteratorAt(uint32 startPos) const;

		//Inline
		inline uint32 CountUTF8Length(const uint8 *src, uint32 &nBytes) const
		{
			nBytes = 0;
			uint32 length = 0;
			uint8 nBytesCP;
			while(this->DecodeUTF8(src, nBytesCP))
			{
				length++;
				nBytes += nBytesCP;
				src += nBytesCP;
			}

			return length;
		}

		inline uint32 CountUTF16Length(const uint16 *src, uint32 &nBytes) const
		{
			nBytes = 0;
			uint32 length = 0;
			bool isSurrogate;
			while (this->DecodeUTF16(src, isSurrogate))
			{
				length++;
				nBytes += 2 * (1 + isSurrogate);
				src += 1 + isSurrogate;
			}

			return length;
		}

		inline uint32 CountUTF16Size(const uint16* src, uint32 nChars) const
		{
			uint32 nBytes = 0;
			bool isSurrogate;
			while ((nChars--) && this->DecodeUTF16(src, isSurrogate))
			{
				nBytes += 2 * (1 + isSurrogate);
				src += 1 + isSurrogate;
			}

			return nBytes;
		}

		inline uint32 Decode(const byte *src, uint8 &nBytes) const
		{
			if(this->IsUTF8())
				return this->DecodeUTF8(src, nBytes);

			bool isSurrogate;
			uint32 codePoint = this->DecodeUTF16(reinterpret_cast<const uint16 *>(src), isSurrogate);
			nBytes = static_cast<uint8>(2 + (isSurrogate) * 2);
			return codePoint;
		}

		inline uint8 Encode(uint32 codePoint, byte *dest) const
		{
			if(this->IsUTF8())
				return this->EncodeUTF8(codePoint, dest);
			return 2 + 2 * this->EncodeUTF16(codePoint, (uint16 *)dest);
		}

		inline uint32 GetByteOffset() const
		{
			return this->data - this->sharedResource->data;
		}

		inline bool IsNull() const
		{
			return this->data == nullptr; //implies that also sharedResource is nullptr
		}

		inline bool IsUTF16() const
		{
			if (this->sharedResource)
				return !this->sharedResource->isUTF8;
			return false; //no resource means utf8 literal
		}

		inline bool IsUTF8() const
		{
			if(this->sharedResource)
				return this->sharedResource->isUTF8;
			return true; //no resource means utf8 literal
		}

		inline void ResizeAdditional(uint32 additionalSize)
		{
			this->sharedResource->EnsureAdditionalCapacity(additionalSize);
			this->data = this->sharedResource->data; //resize could have moved data.
			// However, a resize means we have exclusive access therefore data was not offset
		}
	};

	//Operators
	inline bool operator==(const char *utf8, const String &rhs)
	{
		return rhs.operator==(utf8);
	}

	inline String operator+(const char8_t *utf8, const String &rhs)
	{
		return String(utf8) + rhs;
	}
}
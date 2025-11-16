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
#include <StdXXTest.hpp>
using namespace StdXX;
using namespace StdXX::CommonFileFormats::XML;
using namespace StdXX::Serialization;

enum class TestEnum
{
	Value1,
	Value2,
};

struct TestObj1
{
	int32 v;
	TestEnum e;
};

struct TestObj2
{
	int32 i;
	String s;
	TestObj1 nested;
};

namespace StdXX::Serialization
{
	template<class ArchiveType>
	void Archive(ArchiveType & ar, TestObj1& obj)
	{
		StaticArray<Tuple<TestEnum, String>, 2> enumMapping = { { {TestEnum::Value1, u8"Value1"}, {TestEnum::Value2, u8"Value2"} } };

		ar & Binding(u8"v", obj.v);
		ar & Binding(u8"e", StringMapping(obj.e, enumMapping));
	}

	template<class ArchiveType>
	void Archive(ArchiveType & ar, TestObj2& obj)
	{
		ar & Binding(u8"i", obj.i);
		ar & Binding(u8"s", obj.s);
		ar & Binding(u8"nested", obj.nested);
	}
}

TEST_SUITE(SerializationTests)
{
	TEST_CASE(SerializeThenDeserializeTestUsingJSON)
	{
		FIFOBuffer buffer;

		TestObj2 obj{34, u8"test", {59, TestEnum::Value1}};

		JSONSerializer serializer(buffer);
		serializer << obj;

		TestObj2 obj2{};
		JSONDeserializer deserializer(buffer, true);
		deserializer >> obj2;

		ASSERT(obj.i == obj2.i, u8"Binding and deserialization should yield same values");
		ASSERT(obj.s == obj2.s, u8"Binding and deserialization should yield same values");
		ASSERT(obj.nested.v == obj2.nested.v, u8"Binding and deserialization should yield same values");
		ASSERT_EQUALS(obj.nested.e, obj2.nested.e);
	}

	TEST_CASE(SerializeThenDeserializeTestUsingXML)
	{
		FIFOBuffer buffer;

		TestObj2 obj{34, u8"test", {59, TestEnum::Value2}};

		XMLSerializer serializer(buffer);
		serializer << Binding{u8"TestObj", obj};

		TestObj2 obj2{};
		XMLDeserializer deserializer(buffer);
		deserializer >> Binding{u8"TestObj", obj2};

		ASSERT(obj.i == obj2.i, u8"Binding and deserialization should yield same values");
		ASSERT(obj.s == obj2.s, u8"Binding and deserialization should yield same values");
		ASSERT(obj.nested.v == obj2.nested.v, u8"Binding and deserialization should yield same values");
		ASSERT_EQUALS(obj.nested.e, obj2.nested.e);
	}
};
/*
 * Copyright (c) 2020 Amir Czwink (amir130@hotmail.de)
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
#include <Std++Test.hpp>
using namespace StdXX;
using namespace StdXX::CommonFileFormats::XML;
using namespace StdXX::Serialization;

struct TestObj1
{
	int32 v;
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
		ar & Binding(u8"v", obj.v);
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
	template <typename SerializerType>
	void Execute()
	{
		FIFOBuffer buffer;

		TestObj2 obj{34, u8"test", {59}};

		SerializerType serializer(buffer);
		serializer << Binding{u8"TestObj", obj};

		TestObj2 obj2{};
		XmlDeserializer deserializer(buffer);
		deserializer >> Binding{u8"TestObj", obj2};

		ASSERT(obj.i == obj2.i, u8"Binding and deserialization should yield same values");
		ASSERT(obj.s == obj2.s, u8"Binding and deserialization should yield same values");
		ASSERT(obj.nested.v == obj2.nested.v, u8"Binding and deserialization should yield same values");
	}

	TEST_CASE(SerializeThenDeserializeTestUsingXML)
	{
		//Execute<JsonSerializer>();
		Execute<XmlSerializer>();
	}
};
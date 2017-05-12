/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <ACStdLib.hpp>

//Definitions
#define _TEST_ADDER_NAME(name, line) name ## line
#define _TEST_ADDER_NAME2(name, line) _TEST_ADDER_NAME(name, line)
#define TEST_SUITE(name) namespace __##name
#define TEST(name) void name(); namespace {static ACStdLibTest::TestAdder _TEST_ADDER_NAME2(__unique, __LINE__)(#name, name);} void name()

//Test classes
namespace ACStdLibTest
{
    //Forward declarations
    class TestSuite;

    class TestManager
    {
        struct Test
        {
            const char *pName;
            void (*pTest)();
        };
    private:
        //Members
        ACStdLib::LinkedList<Test> tests;

        //Constructor
        inline TestManager()
        {
        }

    public:
        //Inline
        inline void AddTest(const char *pName, void (*pTest)())
        {
            Test t;

            t.pName = pName;
            t.pTest = pTest;

            this->tests.InsertTail(t);
        }

        inline bool RunAllTests()
        {
            ACStdLib::stdOut << "Running " << this->tests.GetNumberOfElements() << " tests..." << ACStdLib::endl;
            for(Test &refTest : this->tests)
            {
                ACStdLib::stdOut << "Running test: " << refTest.pName << "..." << ACStdLib::endl;
                refTest.pTest();

                ACStdLib::stdOut << "\tPassed!" << ACStdLib::endl;
            }

            return true;
        }

        //Functions
        static inline TestManager &GetInstance()
        {
            static TestManager mgr;

            return mgr;
        }
    };

    class TestAdder
    {
    public:
        //Constructor
        inline TestAdder(const char *pName, void (*pTest)())
        {
            ACStdLibTest::TestManager::GetInstance().AddTest(pName, pTest);
        }
    };
}
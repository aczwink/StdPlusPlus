/*
 * Copyright (c) 2017-2018 Amir Czwink (amir130@hotmail.de)
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
#include <Std++.hpp>

//Definitions
#define _TEST_ADDER_NAME(name, line) name ## line
#define _TEST_ADDER_NAME2(name, line) _TEST_ADDER_NAME(name, line)
#define TEST_SUITE(name) namespace {static StdPlusPlusTest::TestSuiteSetter _TEST_ADDER_NAME2(__unique, __LINE__)(#name);} namespace __##name
#define TEST(name) void name(); namespace {static StdPlusPlusTest::TestAdder _TEST_ADDER_NAME2(__unique, __LINE__)(#name, name);} void name()

//Test classes
namespace StdPlusPlusTest
{
    //Forward declarations
    class TestSuite;

    class TestManager
    {
        struct Test
        {
        	const char* testSuiteName;
            const char *name;
            void (*testFunction)();
        };
    private:
        //Members
    	const char* currentTestSuite;
        StdXX::LinkedList<Test> tests;

        //Constructor
        inline TestManager()
        {
        }

    public:
        //Inline
        inline void AddTest(const char *pName, void (*pTest)())
        {
            Test t;

            t.testSuiteName = this->currentTestSuite;
            t.name = pName;
            t.testFunction = pTest;

            this->tests.InsertTail(t);
        }

        inline bool RunAllTests()
        {
            StdXX::stdOut << u8"Running " << this->tests.GetNumberOfElements() << " tests..." << StdXX::endl;
            for(Test &test : this->tests)
			{
				StdXX::stdOut << "Running test: " << test.testSuiteName << u8"::" << test.name << "..." << StdXX::endl;
				try
				{
					test.testFunction();
					StdXX::stdOut << "\tPassed!" << StdXX::endl;
				}
				catch(const StdXX::Exception &e)
				{
					StdXX::stdOut << u8"\tFailed! Caught exception: " << e.GetDescription() << StdXX::endl;
				}
				catch(const StdXX::Error &e)
				{
					StdXX::stdOut << u8"\tFailed! Caught error: " << e.GetDescription() << StdXX::endl;
				}
				catch (...)
				{
					StdXX::stdOut << u8"\tFailed! Caught uncaught exception (not StdXX)!" << StdXX::endl;
				}
            }

            return true;
        }

        inline void SetTestSuiteName(const char* name)
		{
			this->currentTestSuite = name;
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
            StdPlusPlusTest::TestManager::GetInstance().AddTest(pName, pTest);
        }
    };

    class TestSuiteSetter
	{
	public:
		//Constructor
		inline TestSuiteSetter(const char *name)
		{
			StdPlusPlusTest::TestManager::GetInstance().SetTestSuiteName(name);
		}
	};
}
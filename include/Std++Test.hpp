/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
#include <StdXX.hpp>

//Definitions
#define _TEST_ADDER_NAME(name, line) name ## line
#define _TEST_ADDER_NAME2(name, line) _TEST_ADDER_NAME(name, line)
#define TEST_SUITE(name) namespace {static StdPlusPlusTest::TestSuiteSetter _TEST_ADDER_NAME2(__unique, __LINE__)(#name);} namespace __##name
#define TEST_CASE(name) void name(); namespace {static StdPlusPlusTest::TestAdder _TEST_ADDER_NAME2(__unique, __LINE__)(#name, name);} void name()

//Test classes
namespace StdPlusPlusTest
{
    //Forward declarations
    class TestSuite;

    class TestManager
    {
        struct Test
        {
            const char *name;
            void (*testFunction)();
        };
    public:
        //Inline
        inline void AddTest(const char *pName, void (*pTest)())
        {
            Test t;
            t.name = pName;
            t.testFunction = pTest;

            this->testSuites[this->currentTestSuite].Push(StdXX::Move(t));
        }

        inline void RunAllTests()
        {
            for(const auto& kv : this->testSuites)
			{
				this->ExecuteTestSuite(kv.key);
            }
            this->PrintStatistics();
        }

        inline bool RunTestCase(const StdXX::String& testSuiteName, const StdXX::String& testCaseName)
        {
        	if(!this->testSuites.Contains(testSuiteName))
        		return false;

	        const StdXX::DynamicArray<Test>& testCases = this->testSuites[testSuiteName];
	        bool found = false;
	        for(const Test& testCase : testCases)
	        {
	        	if(StdXX::String(testCase.name) == testCaseName)
		        {
		        	found = true;
		        	this->ExecuteTestCase(testCase);
		        	break;
		        }
	        }
	        if(!found)
	        	return false;

	        this->PrintStatistics();
	        return true;
        }

        inline bool RunTestSuite(const StdXX::String& testSuiteName)
        {
        	if(!this->testSuites.Contains(testSuiteName))
        		return false;

        	this->ExecuteTestSuite(testSuiteName);
        	this->PrintStatistics();
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

    private:
	    //Members
    	uint32 nTestsTotallyExecuted;
        uint32 nSuccessfulTests;
	    const char* currentTestSuite;
	    StdXX::Map<StdXX::String, StdXX::DynamicArray<Test>> testSuites;

	    //Constructor
	    inline TestManager()
	    {
		    this->nTestsTotallyExecuted = 0;
		    this->nSuccessfulTests = 0;
	    }

	    //Inline
	    inline void ExecuteTestCase(const Test& testCase)
	    {
	    	StdXX::TextWriter textWriter(StdXX::stdOut, StdXX::TextCodecType::UTF8);
		    textWriter << u8"\tRunning test: " << testCase.name << u8"..." << StdXX::endl;
		    this->nTestsTotallyExecuted++;
		    try
		    {
			    testCase.testFunction();
			    textWriter << "\t\tPassed!" << StdXX::endl;
			    this->nSuccessfulTests++;
		    }
		    catch (const StdXX::Exception &e)
		    {
			    textWriter << u8"\t\tFailed! Caught exception: " << e.ToString() << StdXX::endl;
		    }
		    catch (const StdXX::Error &e)
		    {
			    textWriter << u8"\t\tFailed! Caught error: " << e.ToString() << StdXX::endl;
		    }
		    catch (...)
		    {
			    textWriter << u8"\t\tFailed! Caught uncaught exception (not StdXX)!" << StdXX::endl;
		    }
	    }

	    inline void ExecuteTestSuite(const StdXX::String& testSuiteName)
	    {
		    StdXX::TextWriter textWriter(StdXX::stdOut, StdXX::TextCodecType::UTF8);
		    textWriter << u8"Running tests for suite: " << testSuiteName << StdXX::endl;
		    const StdXX::DynamicArray<Test>& testCases = this->testSuites[testSuiteName];
		    for(const Test& testCase : testCases)
		    {
		    	this->ExecuteTestCase(testCase);
		    }
	    }

	    inline void PrintStatistics()
	    {
		    StdXX::TextWriter textWriter(StdXX::stdOut, StdXX::TextCodecType::UTF8);
		    textWriter << u8"Performed " << this->nTestsTotallyExecuted << u8" tests. " << this->nSuccessfulTests << u8" were successful, " << (this->nTestsTotallyExecuted - this->nSuccessfulTests) << u8" failed." << StdXX::endl;
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
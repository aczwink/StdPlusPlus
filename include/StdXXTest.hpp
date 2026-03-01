/*
 * Copyright (c) 2017-2026 Amir Czwink (amir130@hotmail.de)
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
#define TEST_SUITE(name) namespace {static StdPlusPlusTest::TestSuiteSetter _TEST_ADDER_NAME2(__unique, __LINE__)(#name, __FILE__);} namespace __##name
#define TEST_CASE(name) void name(); namespace {static StdPlusPlusTest::TestAdder _TEST_ADDER_NAME2(__unique, __LINE__)(#name, name);} void name()

//Test classes
namespace StdPlusPlusTest
{
    //Forward declarations
    class TestSuite;

	class TestReporter
	{
	public:
		//Destructor
		virtual ~TestReporter(){};

		virtual void BeginTestCaseExecution(const StdXX::String& testCaseName) = 0;
		virtual void BeginTestSuiteExecution(const StdXX::String& testSuiteName, const StdXX::String& testSuiteFilePath) = 0;
		virtual void TestCaseExecutionFailedWithError(const StdXX::Error& e) = 0;
		virtual void TestCaseExecutionFailedWithException(const StdXX::Exception& e) = 0;
		virtual void TestCaseExecutionFailedWithUnknownError() = 0;
		virtual void TestCaseExecutionPassed() = 0;
		virtual void TestSuitesExecutionFinished() = 0;
	};

    class TestManager
    {
        struct Test
        {
            const char *name;
            void (*testFunction)();
        };
		struct TestSuite
		{
			const char* filePath;
			StdXX::DynamicArray<Test> testCases;
		};
    public:
        //Inline
        inline void AddTest(const char *pName, void (*pTest)())
        {
            Test t;
            t.name = pName;
            t.testFunction = pTest;

            this->testSuites[this->currentTestSuite].testCases.Push(StdXX::Move(t));
        }

        inline void RunAllTests()
        {
            for(const auto& kv : this->testSuites)
			{
				this->ExecuteTestSuite(kv.key);
            }
			this->reporter->TestSuitesExecutionFinished();
        }

        inline bool RunTestCase(const StdXX::String& testSuiteName, const StdXX::String& testCaseName)
        {
        	if(!this->testSuites.Contains(testSuiteName))
        		return false;

	        const StdXX::DynamicArray<Test>& testCases = this->testSuites[testSuiteName].testCases;
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

			this->reporter->TestSuitesExecutionFinished();
	        return true;
        }

        inline bool RunTestSuite(const StdXX::String& testSuiteName)
        {
        	if(!this->testSuites.Contains(testSuiteName))
        		return false;

        	this->ExecuteTestSuite(testSuiteName);
			this->reporter->TestSuitesExecutionFinished();
        	return true;
        }

		inline void SetReporter(StdXX::UniquePointer<TestReporter>&& reporter)
		{
			this->reporter = StdXX::Move(reporter);
		}

        inline void SetTestSuite(const char* name, const char* filePath)
		{
			this->currentTestSuite = name;

			this->testSuites[name].filePath = filePath;
		}

        //Functions
        static inline TestManager &GetInstance()
        {
            static TestManager mgr;

            return mgr;
        }

    private:
	    //Members
	    const char* currentTestSuite;
	    StdXX::BinaryTreeMap<StdXX::String, TestSuite> testSuites;
		StdXX::UniquePointer<TestReporter> reporter;

	    //Inline
	    inline void ExecuteTestCase(const Test& testCase)
	    {
			this->reporter->BeginTestCaseExecution(testCase.name);
		    try
		    {
			    testCase.testFunction();
				this->reporter->TestCaseExecutionPassed();
		    }
		    catch (const StdXX::Exception &e)
		    {
				this->reporter->TestCaseExecutionFailedWithException(e);
		    }
		    catch (const StdXX::Error &e)
		    {
				this->reporter->TestCaseExecutionFailedWithError(e);
		    }
		    catch (...)
		    {
				this->reporter->TestCaseExecutionFailedWithUnknownError();
		    }
	    }

	    inline void ExecuteTestSuite(const StdXX::String& testSuiteName)
	    {
			const auto& suite = this->testSuites[testSuiteName];

			this->reporter->BeginTestSuiteExecution(testSuiteName, suite.filePath);

		    for(const Test& testCase : suite.testCases)
		    {
		    	this->ExecuteTestCase(testCase);
		    }
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
		inline TestSuiteSetter(const char *name, const char* filePath)
		{
			StdPlusPlusTest::TestManager::GetInstance().SetTestSuite(name, filePath);
		}
	};
}
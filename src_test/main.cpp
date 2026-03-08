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
#include <StdXXTest.hpp>
//Namespaces
using namespace StdXX;

class MochaJSONTestReporter : public StdPlusPlusTest::TestReporter
{
	struct TestResult
	{
		Optional<String> error;
		uint64 executionDuration;
		String filePath;
		String testSuiteTitle;
		String testCaseTitle;
	};
public:
	//Constructor
	inline MochaJSONTestReporter() : startTime(DateTime::Now())
	{
	}

	//Public methods
	void BeginTestCaseExecution(const String& testCaseName) override
	{
		this->currentTestCaseName = testCaseName;
		this->clock.Start();
	}

	void BeginTestSuiteExecution(const String& testSuiteName, const String& testSuiteFilePath) override
	{
		this->currentTestSuiteName = testSuiteName;
		this->currentTestSuiteFilePath = testSuiteFilePath;
	}

	void TestCaseExecutionFailedWithError(const Error& e) override
	{
		this->AddFailedTestResult(u8"ERROR: " + e.ToString());
	}

	void TestCaseExecutionFailedWithException(const Exception& e) override
	{
		this->AddFailedTestResult(u8"EXCEPTION: " + e.ToString());
	}

	void TestCaseExecutionFailedWithUnknownError() override
	{
		this->AddFailedTestResult(u8"uncaught exception (not StdXX)");
	}

	void TestCaseExecutionPassed() override
	{
		TestResult result;

		result.executionDuration = this->clock.GetElapsedMicroseconds();
		result.filePath = this->currentTestSuiteFilePath;
		result.testCaseTitle = this->currentTestCaseName;
		result.testSuiteTitle = this->currentTestSuiteName;

		this->results.Push(Move(result));
	}
	
	void TestSuitesExecutionFinished() override
	{
		DateTime endTime = DateTime::Now();

		CommonFileFormats::JSONValue tests = CommonFileFormats::JSONValue::Array();
		CommonFileFormats::JSONValue failures = CommonFileFormats::JSONValue::Array();
		CommonFileFormats::JSONValue passes = CommonFileFormats::JSONValue::Array();
		BinaryTreeSet<String> testSuites;

		for (const auto& result : this->results)
		{
			testSuites.Insert(result.testSuiteTitle);

			auto mapped = this->MapTestResult(result);
			tests.Push(mapped);

			if (result.error.HasValue())
				failures.Push(mapped);
			else
				passes.Push(mapped);
		}

		CommonFileFormats::JSONValue stats = CommonFileFormats::JSONValue::Object();
		stats[u8"suites"] = testSuites.GetNumberOfElements();
		stats[u8"tests"] = tests.ArrayValue().GetNumberOfElements();
		stats[u8"passes"] = passes.ArrayValue().GetNumberOfElements();
		stats[u8"pending"] = 0;
		stats[u8"failures"] = failures.ArrayValue().GetNumberOfElements();
		stats[u8"start"] = this->startTime.ToISOString();
		stats[u8"end"] = endTime.ToISOString();
		stats[u8"duration"] = (endTime.ToUnixTimestamp() - this->startTime.ToUnixTimestamp())*1000;

		CommonFileFormats::JSONValue document = CommonFileFormats::JSONValue::Object();
		document[u8"stats"] = stats;
		document[u8"tests"] = Move(tests);
		document[u8"pending"] = CommonFileFormats::JSONValue::Array();
		document[u8"failures"] = Move(failures);
		document[u8"passes"] = Move(passes);

		stdOut << document.Dump();
	}

private:
	//State
	Clock clock;
	String currentTestCaseName;
	String currentTestSuiteName;
	String currentTestSuiteFilePath;
	DateTime startTime;
	DynamicArray<TestResult> results;

	//Methods
	void AddFailedTestResult(const String& errorMessage)
	{
		TestResult result;

		result.error = errorMessage;
		result.executionDuration = this->clock.GetElapsedMicroseconds();
		result.filePath = this->currentTestSuiteFilePath;
		result.testCaseTitle = this->currentTestCaseName;
		result.testSuiteTitle = this->currentTestSuiteName;

		this->results.Push(Move(result));
	}

	CommonFileFormats::JSONValue MapTestResult(const TestResult& testResult)
	{
		CommonFileFormats::JSONValue result = CommonFileFormats::JSONValue::Object();

		result[u8"currentRetry"] = 0;
		result[u8"duration"] = this->clock.GetElapsedMilliseconds();
		result[u8"file"] = testResult.filePath;
		result[u8"fullTitle"] = testResult.testSuiteTitle + u8"::" + testResult.testCaseTitle;
		result[u8"title"] = testResult.testCaseTitle;

		auto err = CommonFileFormats::JSONValue::Object();

		if (testResult.error.HasValue())
		{
			err[u8"message"] = *testResult.error;
			err[u8"stack"] = u8"";
		}
		result[u8"err"] = Move(err);

		return result;
	}
};

class StdOutTestReporter : public StdPlusPlusTest::TestReporter
{
public:
	//Constructor
	inline StdOutTestReporter() : textWriter(stdOut, TextCodecType::UTF8)
	{
		this->nTestsTotallyExecuted = 0;
		this->nSuccessfulTests = 0;
	}

	//Public methods
	void BeginTestCaseExecution(const String& testCaseName) override
	{
		this->textWriter << u8"\tRunning test: " << testCaseName << u8"..." << endl;
		this->nTestsTotallyExecuted++;
	}

	void BeginTestSuiteExecution(const String& testSuiteName, const String& testSuiteFilePath) override
	{
		this->textWriter << u8"Running tests for suite: " << testSuiteName << endl;
	}

	void TestCaseExecutionFailedWithError(const StdXX::Error& e) override
	{
		this->textWriter << u8"\t\tFailed! Caught error: " << e.ToString() << endl;
	}

	void TestCaseExecutionFailedWithException(const Exception& e) override
	{
		this->textWriter << u8"\t\tFailed! Caught exception: " << e.ToString() << endl;
	}

	void TestCaseExecutionFailedWithUnknownError() override
	{
		this->textWriter << u8"\t\tFailed! Caught uncaught exception (not StdXX)!" << endl;
	}

	void TestCaseExecutionPassed() override
	{
		this->textWriter << "\t\tPassed!" << endl;
		this->nSuccessfulTests++;
	}

	void TestSuitesExecutionFinished() override
	{
		this->textWriter << u8"Performed " << this->nTestsTotallyExecuted << u8" tests. " << this->nSuccessfulTests << u8" were successful, " << this->FailedTestsCount() << u8" failed." << StdXX::endl;
	}
	
private:
	//State
	TextWriter textWriter;
	uint32 nTestsTotallyExecuted;
	uint32 nSuccessfulTests;

	//Properties
	inline uint32 FailedTestsCount() const
	{
		return this->nTestsTotallyExecuted - this->nSuccessfulTests;
	}
};

//Functions
int32 Main(const String &programName, const FixedArray<String> &args)
{
	CommandLine::Parser parser(programName);

	parser.AddHelpOption();

	CommandLine::OptionWithArgument suiteOpt(u8's', u8"suite", u8"Suite that should be executed");
	parser.AddOption(suiteOpt);

	CommandLine::OptionWithArgument caseOpt(u8'c', u8"case", u8"Test case that should be executed");
	parser.AddOption(caseOpt);

	CommandLine::Option formatOpt(u8'm', u8"mocha-json", u8"Output as mocha json format");
	parser.AddOption(formatOpt);

	if (!parser.Parse(args))
	{
		parser.PrintHelp();
		return EXIT_FAILURE;
	}

	const CommandLine::MatchResult& result = parser.ParseResult();

	if (result.IsActivated(formatOpt))
		StdPlusPlusTest::TestManager::GetInstance().SetReporter(new MochaJSONTestReporter);
	else
		StdPlusPlusTest::TestManager::GetInstance().SetReporter(new StdOutTestReporter);

	if (result.IsActivated(suiteOpt))
	{
		auto suiteName = suiteOpt.Value(result);

		if (result.IsActivated(caseOpt))
		{
			auto testCaseName = caseOpt.Value(result);
			if (!StdPlusPlusTest::TestManager::GetInstance().RunTestCase(suiteName, testCaseName))
			{
				stdErr << u8"ERROR: Could not find test suite or test case." << endl;
				StdPlusPlusTest::TestManager::GetInstance().SetReporter(nullptr);
				return EXIT_FAILURE;
			}
		}
		else
		{
			if (!StdPlusPlusTest::TestManager::GetInstance().RunTestSuite(suiteName))
			{
				stdErr << u8"ERROR: Could not find test suite." << endl;
				StdPlusPlusTest::TestManager::GetInstance().SetReporter(nullptr);
				return EXIT_FAILURE;
			}
		}
	}
	else
	{
		StdPlusPlusTest::TestManager::GetInstance().RunAllTests();
	}

	StdPlusPlusTest::TestManager::GetInstance().SetReporter(nullptr);

	return EXIT_SUCCESS;
}
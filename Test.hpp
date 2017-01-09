#ifndef TEST_H
#define TEST_H

#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

// UNIX-like
#if !defined(_WIN32) && ((defined(__unix__) || defined(__unix)) || (defined(__APPLE__) && defined(__MACH__)))

#	define TEXT_NULL      "\033[0m"
#	define TEXT_BOLD      "\033[1m"
#	define TEXT_MIDDLE    "\033[2m"
#	define TEXT_LIGHT     "\033[3m"
#	define TEXT_UNDERLINE "\033[4m"
#	define TEXT_BINK      "\033[5m"
#	define TEXT_INVERT    "\033[7m"
#	define TEXT_INVISIBLE "\033[8m"

#	define TEXT_BLACK  "\033[30m"
#	define TEXT_RED    "\033[31m"
#	define TEXT_GREEN  "\033[32m"
#	define TEXT_YELLOW "\033[33m"
#	define TEXT_BLUE   "\033[34m"
#	define TEXT_VIOLET "\033[35m"
#	define TEXT_CYAN   "\033[36m"
#	define TEXT_WHITE  "\033[37m"

#	define TEXT_BACKGROUND_BLACK  "\033[40m"
#	define TEXT_BACKGROUND_RED    "\033[41m"
#	define TEXT_BACKGROUND_GREEN  "\033[42m"
#	define TEXT_BACKGROUND_YELLOW "\033[43m"
#	define TEXT_BACKGROUND_BLUE   "\033[44m"
#	define TEXT_BACKGROUND_VIOLET "\033[45m"
#	define TEXT_BACKGROUND_CYAN   "\033[46m"
#	define TEXT_BACKGROUND_WHITE  "\033[47m"

inline void setConsoleText(const char *textColor, const char *backgroundColor) noexcept
{
	std::cout << textColor << backgroundColor;
}

#else // Windows

#include <windows.h>

enum class TextColor: uint8_t
{
	BLACK  = 0u,
	RED    = 12u,
	GREEN  = 10u,
	YELLOW = 14u,
	BLUE   = 1u,
	VIOLET = 13u,
	CYAN   = 11u,
	WHITE  = 15u
};

enum class BackgroundColor: uint8_t
{
	BLACK  = 0u,
	RED    = 12u,
	GREEN  = 10u,
	YELLOW = 14u,
	BLUE   = 1u,
	VIOLET = 13u,
	CYAN   = 11u,
	WHITE  = 15u

};

#	define TEXT_BLACK  TextColor::BLACK
#	define TEXT_RED    TextColor::RED
#	define TEXT_GREEN  TextColor::GREEN
#	define TEXT_YELLOW TextColor::YELLOW
#	define TEXT_BLUE   TextColor::BLUE
#	define TEXT_VIOLET TextColor::VIOLET
#	define TEXT_CYAN   TextColor::CYAN
#	define TEXT_WHITE  TextColor::WHITE

#	define TEXT_BACKGROUND_BLACK  BackgroundColor::BLACK
#	define TEXT_BACKGROUND_RED    BackgroundColor::RED
#	define TEXT_BACKGROUND_GREEN  BackgroundColor::GREEN
#	define TEXT_BACKGROUND_YELLOW BackgroundColor::YELLOW
#	define TEXT_BACKGROUND_BLUE   BackgroundColor::BLUE
#	define TEXT_BACKGROUND_VIOLET BackgroundColor::VIOLET
#	define TEXT_BACKGROUND_CYAN   BackgroundColor::CYAN
#	define TEXT_BACKGROUND_WHITE  BackgroundColor::WHITE

inline void setConsoleText(TextColor textColor, BackgroundColor backgroundColor) noexcept
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			(static_cast<uint8_t>(backgroundColor) << 8) + static_cast<uint8_t>(textColor));
}

#endif

class AbstractTest
{
public:
	AbstractTest(const AbstractTest &) = delete;
	AbstractTest(AbstractTest &&) = delete;

	AbstractTest() :
			countCalls(0),
			testName("NONAME TEST")
	{
	}
	AbstractTest(std::string const &name) :
			countCalls(0),
			testName(name)
	{
	}
	AbstractTest(std::string &&name) :
			countCalls(0),
			testName(name)
	{
	}
	AbstractTest(char const *name) :
			countCalls(0),
			testName(name)
	{
	}

	virtual ~AbstractTest()
	{
	}

	int operator()()
	{
		int returnCode;

		printStart();

		returnCode = runTest();
		if(returnCode)
		{
			printMessageFalse();
		}
		else
		{
			printMessageTrue();
		}

		printEnd();

		++countCalls;

		return returnCode;
	}

protected:
	unsigned int getCountCalls() const
	{
		return countCalls;
	}

	virtual int runTest() = 0;

private:
	void printStart()
	{
		std::cout << "> ";
		setConsoleText(TEXT_CYAN, TEXT_BACKGROUND_BLACK);
		std::cout << "Start" " " << testName << "\n";
		setConsoleText(TEXT_WHITE, TEXT_BACKGROUND_BLACK);
	}

	void printEnd()
	{
		std::cout << "> ";
		setConsoleText(TEXT_CYAN, TEXT_BACKGROUND_BLACK);
		std::cout << "End" " " << testName << "\n\n";
		setConsoleText(TEXT_WHITE, TEXT_BACKGROUND_BLACK);
	}

	void printMessageTrue()
	{
		std::cout << "----> ";
		setConsoleText(TEXT_GREEN, TEXT_BACKGROUND_BLACK);
		std::cout << "SUCCESS TEST\n";
		setConsoleText(TEXT_WHITE, TEXT_BACKGROUND_BLACK);
	}

	void printMessageFalse()
	{
		std::cout << "----> ";
		setConsoleText(TEXT_RED, TEXT_BACKGROUND_BLACK);
		std::cout << "FAILED TEST\n";
		setConsoleText(TEXT_WHITE, TEXT_BACKGROUND_BLACK);
	}

	unsigned int countCalls;
	std::string testName;

	friend class TestAggregator;
};

class TestAggregator
{
public:
	TestAggregator(const TestAggregator &) = delete;
	TestAggregator(TestAggregator &&) = delete;

	TestAggregator() : aggregatorName("NONAME TEST AGGREGATOR")
	{
	}

	TestAggregator(char const *name) : aggregatorName(name)
	{
	}

	TestAggregator(std::string const &name) : aggregatorName(name)
	{
	}

	TestAggregator(std::string &&name) : aggregatorName(name)
	{
	}

	virtual ~TestAggregator()
	{
	}

	void start()
	{
		printStart();
		startTests();
		printEnd();
	}

	void pushBackTest(AbstractTest *test, int isEnable = 1)
	{
		vtests.push_back(test);
		enableTests.push_back(isEnable);
	}

	virtual void printStatistic() const
	{
	}

protected:
	std::vector<int> returnCodes;
	std::vector<int> enableTests;

	virtual void startTests()
	{
		AbstractTest *test;
		for(auto it = vtests.begin(); it != vtests.end(); ++it)
		{
			test = *it;
			returnCodes.push_back((*test)());
		}
	}

	void printStart()
	{
		std::cout << "-------------------- ";
		setConsoleText(TEXT_YELLOW, TEXT_BACKGROUND_BLACK);
		std::cout << aggregatorName;
		setConsoleText(TEXT_WHITE, TEXT_BACKGROUND_BLACK);
		std::cout << " --------------------" "\n\n";
	}

	void printEnd()
	{
		std::cout << "-------------------- ";
		setConsoleText(TEXT_YELLOW, TEXT_BACKGROUND_BLACK);
		std::cout << "END";
		setConsoleText(TEXT_WHITE, TEXT_BACKGROUND_BLACK);
		std::cout << " --------------------" "\n";
	}

	std::string getName() const
	{
		return aggregatorName;
	}
	std::string &getName()
	{
		return aggregatorName;
	}

	void setName(const std::string &name)
	{
		aggregatorName = name;
	}
	void setName(std::string &&name)
	{
		aggregatorName = name;
	}

private:
	std::string aggregatorName;
	std::vector<AbstractTest *> vtests;
};

#endif // TEST_H

#include "Test.hpp"

// implementing tests
class SuccessTest: public AbstractTest
{
public:
	SuccessTest() : AbstractTest("SuccessTest")
	{
	}

	virtual ~SuccessTest()
	{
	}

protected:
	virtual int runTest()
	{
		return 0;
	}
};

class FailTest: public AbstractTest
{
public:
	FailTest() : AbstractTest("FailTest")
	{
	}

	virtual ~FailTest()
	{
	}

protected:
	virtual int runTest()
	{
		return 1;
	}
};

// implementing Aggregator
class Tests: public TestAggregator
{
public:
	Tests() : TestAggregator("Example Tests")
	{
		pushBackTest(&successTest);
		pushBackTest(&failTest);
	}

	virtual ~Tests()
	{
	}

private:
	SuccessTest successTest;
	FailTest failTest;
};

// start
int main()
{
	Tests tests;
	tests.start();
	return 0;
}

# Test
Little header-only test framework

# Usage
## Implementation
Success test:
```
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
```

Fail test:
```
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
```

Aggregator:
```
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
```

Main function:
```
int main()
{
  Tests tests;
  tests.start();
  return 0;
}
```

## Output
```
-------------------- Example Tests --------------------

> Start SuccessTest
----> SUCCESS TEST
> End SuccessTest

> Start FailTest
----> FAILED TEST
> End FailTest

-------------------- END --------------------
```

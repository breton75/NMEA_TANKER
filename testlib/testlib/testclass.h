#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QObject>

#include "../testapp/abstract_test.h"

class TestClass: public AbstractTest
{
public:
  TestClass();

private:
  void foo();


};

#endif // TESTCLASS_H

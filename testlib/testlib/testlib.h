#ifndef TESTLIB_H
#define TESTLIB_H

#include "testlib_global.h"
#include "../testapp/abstract_test.h"

class TESTLIBSHARED_EXPORT Testlib: public AbstractTest
{
public:
  Testlib();

private:
  QString foo();
};

extern "C" {

  TESTLIBSHARED_EXPORT AbstractTest* create();

}

#endif // TESTLIB_H

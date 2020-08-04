#include "testlib.h"


Testlib::Testlib()
{
}

QString Testlib::foo()
{
  return "jopa";
}

AbstractTest* create()
{
  return new Testlib();
}

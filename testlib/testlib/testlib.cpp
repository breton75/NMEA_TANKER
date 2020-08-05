#include "testlib.h"


Vir1tualDevice::Vir1tualDevice():
  ad::SvAbstractDevice()
{
}

QString Vir1tualDevice::foo()
{
  return "jopa";
}

ad::SvAbstractDevice* create()
{
  return new Vir1tualDevice();
}

#ifndef TESTLIB_H
#define TESTLIB_H

#include "testlib_global.h"
#include "../testapp/abstract_test.h"
#include "../../global/sv_abstract_device.h"
#include "../../../svlib/sv_abstract_logger.h"


namespace vir {

  #pragma pack(push,1)
  struct Header
  {
    quint8  client_addr;
    quint8  func_code;
    quint8  ADDRESS;
    quint8  OFFSET;
    quint16 register_count;
    quint8  byte_count;
  };
  #pragma pack(pop)

//  bool process_data(ad::SvAbstractDevice* device, ad::BUFF* buff, ad::DATA* data, Header* header);

}

enum AvailableIfces {
  RS,
  RS485,
  UDP
};

const QMap<QString, AvailableIfces> ifcesMap = {{"RS",    AvailableIfces::RS},
                                                {"RS485", AvailableIfces::RS485},
                                                {"UDP",   AvailableIfces::UDP}};

class Vir1tualDevice: public ad::SvAbstractDevice
{
//  Q_OBJECT

public:
  Vir1tualDevice();
//  ~Vir1tualDevice();

private:
  QString foo();

  virtual void create_new_thread() throw(SvException) {}

  bool setup(const ad::DeviceInfo& info) {}

  bool open() {}

  void close() {}

};

class TestlibGenericThread: public ad::SvAbstractDeviceThread
{
  Q_OBJECT

public:
  TestlibGenericThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr):
    ad::SvAbstractDeviceThread(device, logger)
  {   }

protected:

  void process_data() {}

};

class TestlibUDPThread: public TestlibGenericThread
{
  Q_OBJECT

public:
  TestlibUDPThread(ad::SvAbstractDevice *device, sv::SvAbstractLogger* logger = nullptr):
    TestlibGenericThread(device, logger)
  {}

  ~TestlibUDPThread();

  void setIfcParams(const QString& params) throw(SvException&) {}

  void open() throw(SvException) {}

  quint64 write(const QByteArray& data) {}

public slots:
  void stop() {}

};

class TestlibSerialThread: public TestlibGenericThread
{
  Q_OBJECT

public:
  TestlibSerialThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr):
    TestlibGenericThread(device, logger)
  {}

  ~TestlibSerialThread();
};

extern "C" {

  TESTLIBSHARED_EXPORT ad::SvAbstractDevice* create();

}

#endif // TESTLIB_H

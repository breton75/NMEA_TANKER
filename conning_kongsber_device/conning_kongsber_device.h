#ifndef CONNING_KONGSBER_DEVICE_H
#define CONNING_KONGSBER_DEVICE_H

#include "conning_kongsber_device_global.h"

#include "../global/sv_abstract_device.h"
#include "../global/sv_signal.h"

#include "../../svlib/sv_abstract_logger.h"
#include "../../svlib/sv_exception.h"
#include "../../svlib/sv_crc.h"

#include "device_params.h"
#include "ifc_serial_params.h"
#include "ifc_udp_params.h"
#include "ifc_test_params.h"

extern "C" {

    CONNING_KONGSBER_SHARED_EXPORT ad::SvAbstractDevice* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace ckng {

  #pragma pack(push,1)
  struct Header
  {
    char start_symbol[1];
    char talker[2];
    char name[3];
    char reference[1];
  };
  #pragma pack(pop)

//  bool process_data(ad::SvAbstractDevice* device, ad::BUFF* buff, ad::DATA* data, Header* header);

}

enum AvailableIfces {
  Undefined = -1,
  RS,
  RS485,
  UDP,
  TEST
};

const QMap<QString, AvailableIfces> ifcesMap = {{"RS",    AvailableIfces::RS},
                                                {"RS485", AvailableIfces::RS485},
                                                {"UDP",   AvailableIfces::UDP},
                                                {"TEST",   AvailableIfces::TEST}};

class ConningKongsberDevice: public ad::SvAbstractDevice
{
  Q_OBJECT

public:
  ConningKongsberDevice(sv::SvAbstractLogger* logger = nullptr);
  ~ConningKongsberDevice();

  bool configure(const ad::DeviceInfo& info);

  bool open();
  void close();

  void create_new_thread() throw(SvException);

//  DeviceParams* params() { return &_params; }

private:
//  DeviceParams dev_params;
//  UdpParams    udp_params;
//  SerialParams serial_params;

  bool is_configured = false;

private slots:
  void deleteThread();

signals:
  void stopThread();

};

class ConningKongsberGenericThread: public ad::SvAbstractDeviceThread
{
  Q_OBJECT

public:
  ConningKongsberGenericThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr):
    ad::SvAbstractDeviceThread(device, logger)
  {   }

protected:
  DeviceParams dev_params;

  ckng::Header header;
  size_t hsz = sizeof(ckng::Header);

  quint8  confirm[8];

  void process_data();

private:
  quint16 parse_data();
  QByteArray confirmation();

  QString m_current_message;

  void func_1();
  void func_2();
  void func_3();
  void func_4();
  void func_5();
  void func_6();
  void func_7();

};

class ConningKongsberUDPThread: public ConningKongsberGenericThread
{
  Q_OBJECT

public:
  ConningKongsberUDPThread(ad::SvAbstractDevice *device, sv::SvAbstractLogger* logger = nullptr);
  ~ConningKongsberUDPThread();

  void open() throw(SvException);

  quint64 write(const QByteArray& data);

  void conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException);

private:
  QUdpSocket socket;

  UdpParams    ifc_params;

  void run() Q_DECL_OVERRIDE;

public slots:
  void stop();

};

class ConningKongsberSerialThread: public ConningKongsberGenericThread
{
  Q_OBJECT

public:
  ConningKongsberSerialThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr);
  ~ConningKongsberSerialThread();

  void open() throw(SvException);

  quint64 write(const QByteArray& data);

  void conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException);

private:
  QSerialPort port;

  SerialParams ifc_params;

  void run() Q_DECL_OVERRIDE;

public slots:
  void stop();

};


class ConningKongsberTestThread: public ConningKongsberGenericThread
{
  Q_OBJECT

public:
  ConningKongsberTestThread(ad::SvAbstractDevice *device, sv::SvAbstractLogger* logger = nullptr);
  ~ConningKongsberTestThread();

  void open() throw(SvException);

  quint64 write(const QByteArray& data);

  void conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException);

private:
  VirtualParams    ifc_params;

  void run() Q_DECL_OVERRIDE;

public slots:
  void stop();

};






#endif // CONNING_KONGSBER_DEVICE_H

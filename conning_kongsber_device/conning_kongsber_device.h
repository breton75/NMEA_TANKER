#ifndef CONNING_KONGSBER_DEVICE_H
#define CONNING_KONGSBER_DEVICE_H

#include <QRegularExpression>

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
#include "signal_params.h"

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
    char start_symbol ='$';
    char talker[2];
    char name[3];
    char comma = ',';
  };
  #pragma pack(pop)

  typedef QMap<int, QString> SignalByOffsetMap;
  typedef QMap<QString, SignalByOffsetMap*> SignalsByTypeMap;

  bool parse_signal(SvSignal* signal);

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

  void addSignal(SvSignal* signal) throw(SvException) ;//override;

//  DeviceParams* params() { return &_params; }

private:

  ckng::SignalByOffsetMap D;
  ckng::SignalByOffsetMap A;

  ckng::SignalsByTypeMap signals_by_type;

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
    ad::SvAbstractDeviceThread(device, logger),
    m_head(QString())
  {
    p_header = QString("$%1%2,").arg(dev_params.talker).arg(dev_params.name);
    m_tail.setPattern("[*][0-9a-fA-F][0-9a-fA-F]");
  }

  void setSignalsMap(ckng::SignalsByTypeMap *sbt);

protected:
  DeviceParams dev_params;

  QString p_header;
  size_t hsz = sizeof(ckng::Header);

  ckng::SignalsByTypeMap* signals_by_type;


  void process_data();

private:
//  QString m_current_message;
  QString m_head;
  QRegularExpression m_tail; // = QRegularExpression("/wds/");

  void parseNlog(QString message);
  QByteArray confirmation();


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
  TestParams    ifc_params;

  void run() Q_DECL_OVERRIDE;

public slots:
  void stop();

};






#endif // CONNING_KONGSBER_DEVICE_H

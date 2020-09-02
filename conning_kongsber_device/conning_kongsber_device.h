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

  typedef QMap<int, QString> SignalsMap;

//  typedef QMap<int, QString> SignalsGEN;
//  typedef QMap<int, QString> SignalsXDR;

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

  ckng::SignalsMap SignalsGEN;
  ckng::SignalsMap SignalsXDR;

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
    m_re_header.setPattern("[$]II(XDR|GEN)[, ]+");
    m_re_tail.setPattern("[*][0-9a-fA-F]{2}");
    m_re_full.setPattern("[$]II(?<name>XDR|GEN)[, ]+\\d+[, ]+[\\w+\\d+., ]+[*](?<crc>[0-9a-fA-F]{2})");

    m_re_XDR.setPattern("[$]IIXDR,(?<type>\\d),"
                             "(?<a1>[0-9. ]+)[, ]+(?<a2>[0-9. ]+)[, ]+(?<a3>[0-9. ]+)[, ]+(?<a4>[0-9. ]+)[, ]+"
                             "(?<a5>[0-9. ]+)[, ]+(?<a6>[0-9. ]+)[, ]+(?<a7>[0-9. ]+)[, ]+(?<a8>[0-9. ]+)[, ]+"
                             "(?<a9>[0-9. ]+)[, ]+(?<a10>[0-9. ]+)[, ]+(?<a11>[0-9. ]+)[, ]+(?<a12>[0-9. ]+)[, ]+"
                             "(?<a13>[0-9. ]+)[, ]+(?<a14>[0-9. ]+)[, ]+(?<a15>[0-9. ]+)[*](?<crc>[0-9a-zA-Z]{2})");

    m_re_GEN.setPattern("[$]IIGEN[, ]+(?<type>\\d{4})[, ]+(?<val>[0-9. ]+)[, ]+(?<data>[0-9a-zA-Z]{4})[*](?<crc>[0-9a-zA-Z]{2})");
  }

  void setSignalsMap(ckng::SignalsMap *smapGEN, ckng::SignalsMap *smapXDR);

protected:
  DeviceParams dev_params;

  size_t hsz = sizeof(ckng::Header);

  ckng::SignalsMap* SignalsGEN;
  ckng::SignalsMap* SignalsXDR;

  void process_data();
  void process_signals();

private:
//  QString m_current_message;
  QRegularExpression m_re_full;
  QRegularExpression m_re_XDR;
  QRegularExpression m_re_GEN;
  QRegularExpression m_re_header;
  QRegularExpression m_re_tail; // = QRegularExpression("/wds/");

  QString m_head;

  void parseNlog(const QString &message);
  void parse_GEN(const QString& message);
  void parse_XDR(const QString& message);

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

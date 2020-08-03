#ifndef SV_ABSTRACT_GENERIC_DEVICE_H
#define SV_ABSTRACT_GENERIC_DEVICE_H

#include <QObject>
#include <QUdpSocket>

#include "../../svlib/sv_udp_params.h"
#include "../../svlib/sv_serial_params.h"

#include "sv_abstract_device.h"
#include "device_params.h"

#define MAX_PACKET_SIZE 0xFFFF


class SvAbstractGenericDevice;
class SvAbstractGenericThread;

class SvAbstractSerialThread;
class SvAbstractUdpThread;

struct BUFF
{
  BUFF() {}

  quint8  buf[MAX_PACKET_SIZE];
  quint64 offset = 0;

};

struct DATA
{
  DATA() {}

  qint8  data[MAX_PACKET_SIZE];
  quint8  data_type;
  quint8  data_length;
  quint16 crc;

};



class SvAbstractGenericDevice : public ad::SvAbstractDevice
{
  Q_OBJECT

public:
  SvAbstractGenericDevice(sv::SvAbstractLogger *logger = nullptr);
  ~SvAbstractGenericDevice();

  bool open();
  void close();

//  virtual bool setup(const ad::DeviceInfo& info) = 0;

  sv::log::sender make_dbus_sender();

protected:
  SvException* p_exception;

//  virtual bool create_new_thread() = 0;


private slots:
  void deleteThread();

signals:
  void stopThread();

};

class SvAbstractGenericThread: public ad::SvAbstractDeviceThread
{
  Q_OBJECT

public:
  SvAbstractGenericThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger *logger = nullptr):
    ad::SvAbstractDeviceThread(device, logger)
  ,p_is_active(false)
  {

  }

  virtual ~SvAbstractGenericThread() { }

  virtual void setIfcParams(const QString& params) throw(SvException&) = 0;

  virtual void open() throw(SvException&) = 0;

  virtual quint64 write(const QByteArray& data) = 0;

protected:

  bool p_is_active;

  BUFF p_buff;
  DATA p_data;

  QTimer  p_reset_timer;

  SvException p_exception;

  virtual void process_data() = 0;

public slots:
  virtual void reset_buffer()
  {
    p_buff.offset = 0;
  }

  virtual void stop()
  {
    p_is_active = false;
  }


};


class SvAbstractUdpThread: public SvAbstractGenericThread
{
  Q_OBJECT

public:
  SvAbstractUdpThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger *logger = nullptr);
//  virtual ~SvAbstractUdpThread() override;

//  virtual void setIfcParams(const QString& params) throw(SvException&);

  virtual void open() throw(SvException&);
//  virtual void stop() override;

  virtual quint64 write(const QByteArray& data);

protected:
  QUdpSocket p_socket;
  sv::UdpParams p_ifc_params;

  virtual void run() override;

  virtual void process_data() = 0;

};


class SvAbstractSerialThread: public SvAbstractGenericThread
{
  Q_OBJECT

public:
  SvAbstractSerialThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger *logger = nullptr);
//  virtual ~SvAbstractSerialDeviceThread();

//  virtual void setIfcParams(const QString& params) throw(SvException&);

  virtual void open() throw(SvException&);
  virtual quint64 write(const QByteArray& data);

protected:
  QSerialPort p_port;
  sv::SerialParams p_ifc_params;

  virtual void run() override;

  virtual void process_data() = 0;

};



#endif // SV_ABSTRACT_GENERIC_DEVICE_H

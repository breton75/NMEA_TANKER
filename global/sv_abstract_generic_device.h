#ifndef SV_ABSTRACT_KSUTS_DEVICE_H
#define SV_ABSTRACT_KSUTS_DEVICE_H

#include <QObject>
#include <QUdpSocket>

#include "sv_abstract_device.h"

#include "../../svlib/sv_abstract_logger.h"
#include "../../svlib/sv_udp_params.h"
#include "../../svlib/sv_serial_params.h"
#include "device_params.h"

#define MAX_PACKET_SIZE 0xFFFF

namespace dev {

 class SvAbstractKsutsDevice;
 class SvAbstractKsutsThread;

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

}

class dev::SvAbstractKsutsDevice : public dev::SvAbstractDevice
{
  Q_OBJECT

public:
  SvAbstractKsutsDevice(dev::HardwareType type, sv::SvAbstractLogger *logger = nullptr);
  ~SvAbstractKsutsDevice();

  bool open();
  void close();

  virtual bool setup(const dev::DeviceInfo& info);

  sv::log::sender make_dbus_sender();

protected:
  SvException* p_exception;

  virtual bool create_new_thread() = 0;


private slots:
  void stopThread();
  void deleteThread();

signals:
  void stop_thread();

};

class dev::SvAbstractKsutsThread: public dev::SvAbstractDeviceThread
{
  Q_OBJECT

public:
  SvAbstractKsutsThread(dev::SvAbstractDevice* device, sv::SvAbstractLogger *logger = nullptr):
    dev::SvAbstractDeviceThread(device, logger)
  ,p_is_active(false)
  {

  }

  virtual ~SvAbstractKsutsThread() { }

  virtual void setIfcParams(const QString& params) throw(SvException&) = 0;

  virtual void open() throw(SvException&) = 0;

  virtual quint64 write(const QByteArray& data) = 0;

protected:

  bool p_is_active;

  dev::BUFF p_buff;
  dev::DATA p_data;

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


class dev::SvAbstractUdpThread: public dev::SvAbstractKsutsThread
{
  Q_OBJECT

public:
  SvAbstractUdpThread(dev::SvAbstractDevice* device, sv::SvAbstractLogger *logger = nullptr);
//  virtual ~SvAbstractUdpThread() override;

  virtual void setIfcParams(const QString& params) throw(SvException&);

  virtual void open() throw(SvException&);
//  virtual void stop() override;

  virtual quint64 write(const QByteArray& data);

protected:
  QUdpSocket p_socket;
  sv::UdpParams p_ifc_params;

  virtual void run() override;

  virtual void process_data() = 0;

};


class dev::SvAbstractSerialThread: public dev::SvAbstractKsutsThread
{
  Q_OBJECT

public:
  SvAbstractSerialThread(dev::SvAbstractDevice* device, sv::SvAbstractLogger *logger = nullptr);
//  virtual ~SvAbstractSerialDeviceThread();

  virtual void setIfcParams(const QString& params) throw(SvException&);

  virtual void open() throw(SvException&);
  virtual quint64 write(const QByteArray& data);

protected:
  QSerialPort p_port;
  sv::SerialParams p_ifc_params;

  virtual void run() override;

  virtual void process_data() = 0;

};



#endif // SV_ABSTRACT_KSUTS_DEVICE_H

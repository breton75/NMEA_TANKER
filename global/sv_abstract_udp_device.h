#ifndef SV_ABSTRACT_UDP_DEVICE_H
#define SV_ABSTRACT_UDP_DEVICE_H

#include <QObject>
#include <QUdpSocket>

#include "sv_abstract_device.h"

#include "../../svlib/sv_abstract_logger.h"

#define RESET_INTERVAL 10

namespace dev {

 class SvAbstractUdpDevice;
 class SvAbstractUdpDeviceThread;

}

class dev::SvAbstractUdpDevice : public dev::SvAbstractDevice
{
  Q_OBJECT

public:
  SvAbstractUdpDevice(dev::HardwareType type, sv::SvAbstractLogger* logger = nullptr);
  ~SvAbstractUdpDevice();

  bool open();
  void close();

  virtual bool setConfig(const dev::DeviceConfig& config);
  virtual bool setParams(const QString& params);

//  virtual void setLogger(const sv::SvAbstractLogger* logger);

protected:
  SvException* p_exception;

//  sv::SvAbstractLogger& p_logger;

  virtual void create_new_thread() = 0;

private slots:
  void deleteThread();

};

class dev::SvAbstractUdpDeviceThread: public dev::SvAbstractDeviceThread
{
  Q_OBJECT

public:
  SvAbstractUdpDeviceThread(dev::SvAbstractDevice* device, sv::SvAbstractLogger *logger = nullptr);
  virtual ~SvAbstractUdpDeviceThread();

  virtual void open() throw(SvException&);
  virtual void stop() override;

protected:
  QUdpSocket p_socket;

  dev::SvAbstractDevice *p_device;

  bool is_active;

  quint8  p_buf[512];
  quint64 p_buf_offset = 0;
  quint64 p_bytes_readed = 0;

  QTimer  p_reset_timer;

  SvException p_exception;


  virtual void run() override;

  virtual void treat_data() = 0;

protected slots:
  virtual void reset_buffer();

};

#endif // SV_ABSTRACT_UDP_DEVICE_H

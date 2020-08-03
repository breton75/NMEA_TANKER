#include "sv_abstract_generic_device.h"

SvAbstractGenericDevice::SvAbstractGenericDevice(sv::SvAbstractLogger *logger):
  ad::SvAbstractDevice(logger)
{

}

SvAbstractGenericDevice::~SvAbstractGenericDevice()
{
  emit stopThread();
  deleteLater();
}

/*
bool SvAbstractGenericDevice::setup(const ad::DeviceInfo& info)
{
  p_info = info;

  try {

    p_params = dev::DeviceParams::fromJson(p_info.device_params);

    if(!p_params.isValid)
      p_exception->raise(QString("Неверные параметры устройства: %1").arg(p_info.device_params));

    return true;

  }

  catch(SvException& e) {

    setLastError(e.error);
    return false;
  }
}
*/

bool SvAbstractGenericDevice::open()
{
  try {

    create_new_thread();

    connect(p_thread, &ad::SvAbstractDeviceThread::finished, this, &SvAbstractGenericDevice::deleteThread);
    connect(this, &SvAbstractGenericDevice::stopThread, p_thread, &ad::SvAbstractDeviceThread::stop);
//    connect(p_thread, &dev::SvAbstractDeviceThread::finished, p_thread, &dev::SvAbstractDeviceThread::deleteLater);

    p_thread->open();

    p_thread->start();

    return true;

  } catch(SvException& e) {

    p_last_error = e.error;
//    p_log << sv::log::mtError << sv::log::llError << e.error << sv::log::endl;

    deleteThread();

    return false;

  }
}

void SvAbstractGenericDevice::close()
{
  emit stopThread();

  p_isOpened = false;
}

void SvAbstractGenericDevice::deleteThread()
{
  if(p_thread) delete p_thread;
}

sv::log::sender SvAbstractGenericDevice::make_dbus_sender()
{
  return sv::log::sender::make(p_logger->options().log_sender_name_format,
                               p_info.name,
                               p_info.id);
}


/**         SvAbstractUdpDeviceThread         **/
SvAbstractUdpThread::SvAbstractUdpThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger *logger):
  SvAbstractGenericThread(device, logger)
{

}

//dev::SvAbstractUdpThread::~SvAbstractUdpThread()
//{
//  p_socket.close();
//  dev::SvAbstractKsutsThread::stop();
//}

void SvAbstractUdpThread::open() throw(SvException&)
{
  if(!p_socket.bind(p_ifc_params.recv_port, QAbstractSocket::DontShareAddress))
    throw p_exception.assign(p_socket.errorString());

  // с заданным интервалом сбрасываем буфер, чтобы отсекать мусор и битые пакеты
  p_reset_timer.setInterval(p_device->params()->reset_timeout);
  p_reset_timer.setSingleShot(true);

  connect(&p_socket, SIGNAL(readyRead()), &p_reset_timer, SLOT(start()));
  connect(&p_reset_timer, &QTimer::timeout, this, &dev::SvAbstractKsutsThread::reset_buffer);

  // именно после open!
  p_socket.moveToThread(this);

}

quint64 SvAbstractUdpThread::write(const QByteArray& data)
{
  if(!p_is_active)
    return 0;

  // небольшая задержка перед отправкой подтверждения
  // из-за того, что "шкаф не успевает обработать данные" (c) Гаврилов
  msleep(10);

  if(p_logger) // && p_device->info()->debug_mode)
    *p_logger << static_cast<dev::SvAbstractKsutsDevice*>(p_device)->make_dbus_sender()
              << sv::log::mtDebug
              << sv::log::llDebug
              << sv::log::TimeZZZ << sv::log::out
              << QString(data.toHex())
              << sv::log::endl;

//  QUdpSocket s;
//  quint64 w = s.writeDatagram(data, QHostAddress(p_ifc_params.host), p_ifc_params.send_port);
  quint64 w = p_socket.writeDatagram(data, QHostAddress(p_ifc_params.host), p_ifc_params.send_port);
  p_socket.flush();

  return w;

}

void SvAbstractUdpThread::run()
{
  p_is_active = true;

  while(p_is_active) {

    while(p_is_active && p_socket.waitForReadyRead(1)) {

      while(p_is_active && p_socket.hasPendingDatagrams())
      {
        if(p_socket.pendingDatagramSize() <= 0)
          continue;

        if(p_buff.offset > MAX_PACKET_SIZE)
          reset_buffer();

        if(!p_is_active)
          break;

        /* ... the rest of the datagram will be lost ... */
        p_buff.offset += p_socket.readDatagram((char*)(&p_buff.buf[p_buff.offset]), MAX_PACKET_SIZE - p_buff.offset);

        process_data();

      }
    }
  }

  p_socket.close();

}

/**         SvAbstractSerialThread         **/
SvAbstractSerialThread::SvAbstractSerialThread(ad::SvAbstractDevice *device, sv::SvAbstractLogger *logger):
  SvAbstractGenericThread(device, logger)
{

}

void SvAbstractSerialThread::setIfcParams(const QString& params) throw(SvException&)
{
  try {

    p_ifc_params = sv::SerialParams::fromJsonString(params);

  }
  catch(SvException& e) {

    throw e;
  }
}

void SvAbstractSerialThread::open() throw(SvException&)
{
  p_port.setPortName   (p_ifc_params.portname   );
  p_port.setBaudRate   (p_ifc_params.baudrate   );
  p_port.setStopBits   (p_ifc_params.stopbits   );
  p_port.setFlowControl(p_ifc_params.flowcontrol);
  p_port.setDataBits   (p_ifc_params.databits   );
  p_port.setParity     (p_ifc_params.parity     );

  if(!p_port.open(QIODevice::ReadWrite))
    throw p_exception.assign(p_port.errorString());

  // с заданным интервалом сбрасываем буфер, чтобы отсекать мусор и битые пакеты
  p_reset_timer.setInterval(p_device->params()->reset_timeout);

  connect(&p_port, SIGNAL(readyRead()), &p_reset_timer, SLOT(start()));
  connect(&p_reset_timer, &QTimer::timeout, this, &dev::SvAbstractKsutsThread::reset_buffer);

  // именно после open!
  p_port.moveToThread(this);

}

quint64 SvAbstractSerialThread::write(const QByteArray& data)
{
  if(p_logger) // && p_device->info()->debug_mode)
    *p_logger << static_cast<dev::SvAbstractKsutsDevice*>(p_device)->make_dbus_sender()
              << sv::log::mtDebug
              << sv::log::llDebug
              << sv::log::TimeZZZ << sv::log::out
              << QString(data.toHex())
              << sv::log::endl;

  return p_port.write(data);

}

void SvAbstractSerialThread::run()
{
  p_is_active = true;

  while(p_is_active) {

    while(p_port.waitForReadyRead(1)) {

      if(p_buff.offset > MAX_PACKET_SIZE)
        reset_buffer();

      p_buff.offset += p_port.read((char*)(&p_buff.buf[0] + p_buff.offset), MAX_PACKET_SIZE - p_buff.offset);


      process_data();


    }
  }

  p_port.close();

}

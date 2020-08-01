#include "sv_abstract_ksuts_device.h"

dev::SvAbstractKsutsDevice::SvAbstractKsutsDevice(dev::HardwareType type, sv::SvAbstractLogger *logger):
  dev::SvAbstractDevice(type, logger)
{

}

dev::SvAbstractKsutsDevice::~SvAbstractKsutsDevice()
{
  stopThread();
  deleteLater();
}

bool dev::SvAbstractKsutsDevice::setup(const dev::DeviceInfo& info)
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

bool dev::SvAbstractKsutsDevice::open()
{
  try {

    if(!create_new_thread())
      p_exception->raise(QString("Неизвестный тип интерфейса: %1").arg(p_info.ifc_name));

    p_thread->setIfcParams(p_info.ifc_params);

    connect(p_thread, &dev::SvAbstractDeviceThread::finished, this, &dev::SvAbstractKsutsDevice::deleteThread);
    connect(this, &SvAbstractKsutsDevice::stop_thread, p_thread, &dev::SvAbstractDeviceThread::stop);
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

void dev::SvAbstractKsutsDevice::close()
{
  stopThread();

  p_isOpened = false;
}

void dev::SvAbstractKsutsDevice::stopThread()
{
  // тут надо делать через сигнал слот, иначе ругается что останавливаем сокет или порт из другого потока
  emit stop_thread();

}

void dev::SvAbstractKsutsDevice::deleteThread()
{
  delete p_thread; //->deleteLater();
}

sv::log::sender dev::SvAbstractKsutsDevice::make_dbus_sender()
{
  return sv::log::sender::make(p_logger->options().log_sender_name_format,
                               p_info.name,
                               p_info.index);
}


/**         SvAbstractUdpDeviceThread         **/
dev::SvAbstractUdpThread::SvAbstractUdpThread(dev::SvAbstractDevice* device, sv::SvAbstractLogger *logger):
  dev::SvAbstractKsutsThread(device, logger)
{

}

void dev::SvAbstractUdpThread::setIfcParams(const QString& params) throw(SvException&)
{
  try {

    p_ifc_params = sv::UdpParams::fromJsonString(params);

  }
  catch(SvException& e) {

    throw e;
  }
}

//dev::SvAbstractUdpThread::~SvAbstractUdpThread()
//{
//  p_socket.close();
//  dev::SvAbstractKsutsThread::stop();
//}

void dev::SvAbstractUdpThread::open() throw(SvException&)
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

quint64 dev::SvAbstractUdpThread::write(const QByteArray& data)
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

void dev::SvAbstractUdpThread::run()
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
dev::SvAbstractSerialThread::SvAbstractSerialThread(SvAbstractDevice *device, sv::SvAbstractLogger *logger):
  dev::SvAbstractKsutsThread(device, logger)
{

}

void dev::SvAbstractSerialThread::setIfcParams(const QString& params) throw(SvException&)
{
  try {

    p_ifc_params = sv::SerialParams::fromJsonString(params);

  }
  catch(SvException& e) {

    throw e;
  }
}

void dev::SvAbstractSerialThread::open() throw(SvException&)
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

quint64 dev::SvAbstractSerialThread::write(const QByteArray& data)
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

void dev::SvAbstractSerialThread::run()
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

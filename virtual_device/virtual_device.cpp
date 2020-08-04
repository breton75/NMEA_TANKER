#include "virtual_device.h"


VirtualDevice::VirtualDevice(sv::SvAbstractLogger *logger):
  ad::SvAbstractDevice(logger)
{

}

bool VirtualDevice::setup(const ad::DeviceInfo& info)
{
  p_info = info;

  try {

    _params = DeviceParams::fromJson(p_info.device_params);

    return true;

  }

  catch(SvException& e) {

    setLastError(e.error);
    return false;
  }
}

void VirtualDevice::create_new_thread() throw(SvException)
{
  try {

    switch (ifcesMap.value(p_info.ifc_name)) {

      case AvailableIfces::RS485:
      case AvailableIfces::RS:

//        p_thread = new SvSerialThread(this, p_logger);
//        p_thread->setIfcParams(p_info.ifc_params);

        break;

      case AvailableIfces::UDP:

//        p_thread = new SvUDPThread(this, p_logger);
//        p_thread->setIfcParams(p_info.ifc_params);

        break;

    default:
      p_exception.raise(QString("Неизвестный тип интерфейса: %1").arg(info()->ifc_name));
      break;

    }
  }

  catch(SvException& e) {

    throw e;

  }
}


/** ******* UDP THREAD ************* **/
VirtualDeviceUDPThread::VirtualDeviceUDPThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger):
  VirtualDeviceGenericThread(device, logger)
{
  dev_params = static_cast<VirtualDevice*>(device)->params();
}

VirtualDeviceUDPThread::~VirtualDeviceUDPThread()
{
  deleteLater();
}

void VirtualDeviceUDPThread::setIfcParams(const QString& params) throw(SvException&)
{
  try {

    ifc_params = UdpParams::fromJsonString(params);

  }
  catch(SvException& e) {

    throw e;
  }
}

void VirtualDeviceUDPThread::open() throw(SvException)
{
  if(!socket.bind(ifc_params.recv_port, QAbstractSocket::DontShareAddress))
    throw p_exception.assign(socket.errorString());

  // с заданным интервалом сбрасываем буфер, чтобы отсекать мусор и битые пакеты
  p_reset_timer.setInterval(dev_params->reset_timeout);
  p_reset_timer.setSingleShot(true);

  connect(&socket, SIGNAL(readyRead()), &p_reset_timer, SLOT(start()));
  connect(&p_reset_timer, &QTimer::timeout, this, &ad::SvAbstractDeviceThread::reset_buffer);

  // именно после open!
  socket.moveToThread(this);
}

void VirtualDeviceUDPThread::run()
{
  p_is_active = true;

  while(p_is_active) {

    while(p_is_active && socket.waitForReadyRead(1)) {

      while(p_is_active && socket.hasPendingDatagrams())
      {
        if(socket.pendingDatagramSize() <= 0)
          continue;

        if(p_buff.offset > MAX_PACKET_SIZE)
          reset_buffer();

        if(!p_is_active)
          break;

        /* ... the rest of the datagram will be lost ... */
        p_buff.offset += socket.readDatagram((char*)(&p_buff.buf[p_buff.offset]), MAX_PACKET_SIZE - p_buff.offset);

        process_data();

      }
    }
  }

  socket.close();

}

quint64 VirtualDeviceUDPThread::write(const QByteArray& data)
{
  if(!p_is_active)
    return 0;

  // небольшая задержка перед отправкой подтверждения
  // из-за того, что "шкаф не успевает обработать данные" (c) Гаврилов
  msleep(10);

  if(p_logger) // && p_device->info()->debug_mode)
    *p_logger //<< static_cast<dev::SvAbstractKsutsDevice*>(p_device)->make_dbus_sender()
              << sv::log::mtDebug
              << sv::log::llDebug
              << sv::log::TimeZZZ << sv::log::out
              << QString(data.toHex())
              << sv::log::endl;

//  QUdpSocket s;
//  quint64 w = s.writeDatagram(data, QHostAddress(p_ifc_params.host), p_ifc_params.send_port);
  quint64 w = socket.writeDatagram(data, QHostAddress(ifc_params.host), ifc_params.send_port);
  socket.flush();

  return w;
}

void VirtualDeviceUDPThread::stop()
{
  p_is_active = false;
}


/** ******* Serial THREAD ************* **/
VirtualDeviceSerialThread::VirtualDeviceSerialThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger):
  VirtualDeviceGenericThread(device, logger)
{

}

VirtualDeviceSerialThread::~VirtualDeviceSerialThread()
{
  deleteLater();
}

void VirtualDeviceSerialThread::setIfcParams(const QString& params) throw(SvException&)
{
  try {

    ifc_params = SerialParams::fromJsonString(params);

  }
  catch(SvException& e) {

    throw e;
  }
}

void VirtualDeviceSerialThread::open() throw(SvException)
{

}

quint64 VirtualDeviceSerialThread::write(const QByteArray& data)
{
  if(!p_is_active)
    return 0;

  // небольшая задержка перед отправкой подтверждения
  // из-за того, что "шкаф не успевает обработать данные" (c) Гаврилов
  msleep(10);

  if(p_logger) // && p_device->info()->debug_mode)
    *p_logger //<< static_cast<dev::SvAbstractKsutsDevice*>(p_device)->make_dbus_sender()
              << sv::log::mtDebug
              << sv::log::llDebug
              << sv::log::TimeZZZ << sv::log::out
              << QString(data.toHex())
              << sv::log::endl;

  return port.write(data);

}

void VirtualDeviceSerialThread::stop()
{
  p_is_active = false;
}


/** **** GENERIC FUNCTIONS **** **/

void VirtualDeviceGenericThread::process_data()
{
//  size_t hsz = sizeof(vir::Header);

  if(p_buff.offset >= hsz) {

    memcpy(&header, &p_buff.buf[0], hsz);

    if((header.client_addr != 1) || (header.func_code != 0x10)) {

      reset_buffer();
      return;
    }

    if(p_buff.offset >= hsz + header.byte_count + 2) {

      quint16 current_register = (static_cast<quint16>(header.ADDRESS << 8)) + header.OFFSET;

      if((current_register < static_cast<VirtualDevice*>(p_device)->params()->start_register) ||
         (current_register > static_cast<VirtualDevice*>(p_device)->params()->last_register))
      {
         reset_buffer();
         return;
      }

        if(p_logger) // && p_device->info()->debug_mode)
          *p_logger //<< static_cast<dev::SvAbstractKsutsDevice*>(p_device)->make_dbus_sender()
                    << sv::log::mtDebug
                    << sv::log::llDebug
                    << sv::log::TimeZZZ << sv::log::in
                    << QString(QByteArray((const char*)&p_buff.buf[0], p_buff.offset).toHex())
                    << sv::log::endl;

        // если хоть какие то пакеты сыпятся (для данного получателя), то
        // считаем, что линия передачи в порядке и задаем новую контрольную точку времени
        p_device->setNewLostEpoch();


        switch (header.OFFSET)
        {
          case 0x00:
          case 0x03:
          case 0x05:
          case 0x10:
          case 0x50:
          case 0x90:

                // здесь просто отправляем ответ-квитирование
                write(confirmation());

                break;

          case 0x06:
          case 0xA0:
          case 0xFA:
          {
              // парсим и проверяем crc
              quint16 calc_crc = parse_data();

              if(calc_crc != p_data.crc)
              {
                // если crc не совпадает, то выходим без обработки и ответа
                if(p_logger)
                    *p_logger //<< static_cast<dev::SvAbstractKsutsDevice*>(p_device)->make_dbus_sender()
                              << sv::log::mtError
                              << sv::log::llError
                              << sv::log::TimeZZZ
                              << QString("Ошибка crc! Ожидалось %1, получено %2").arg(calc_crc, 0, 16).arg(p_data.crc, 0, 16)
                              << sv::log::endl;

              }
              else
              {

                // формируем и отправляем ответ-квитирование
                write(confirmation());

                // раскидываем данные по сигналам, в зависимости от типа данных
                switch (p_data.data_type) {

                  case 0x19: func_virtual(); break;

                }
              }

              break;
            }

            default:
                break;
        }

        reset_buffer();

    }
  }
}

quint16 VirtualDeviceGenericThread::parse_data()
{
  // тип данных
  memcpy(&p_data.data_type, &p_buff.buf[0] + hsz, 1);

  // длина данных
  memcpy(&p_data.data_length, &p_buff.buf[0] + hsz + 1, 1);

  // данные
  memcpy(&p_data.data[0], &p_buff.buf[0] + hsz + 2, p_data.data_length);

  // crc полученная
  memcpy(&p_data.crc, &p_buff.buf[0] + hsz + header.byte_count, 2);

  // вычисляем crc из данных
  quint16 crc = CRC::MODBUS_CRC16(&p_buff.buf[0], hsz + header.byte_count);

  return crc;

}

QByteArray VirtualDeviceGenericThread::confirmation()
{
  QByteArray confirm;
  confirm.append((const char*)(&header), 6);

  // вычисляем crc ответа
  quint16 crc = CRC::MODBUS_CRC16((uchar*)(&header), 6);
  confirm.append(quint8(crc & 0xFF));
  confirm.append(quint8(crc >> 8));

  return confirm;

}

void VirtualDeviceGenericThread::func_virtual()
{

}

/** ********** EXPORT ************ **/
VIRTUAL_DEVICESHARED_EXPORT void* create()
{
  ad::SvAbstractDevice* device = NULL; //new VirtualDevice();
  return device;
}


//VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc = QString())
//{
//  QString result = "";

//  switch(ifcesMap.value(ifc.toUpper()))
//  {
//    case RS:
//    case RS485:

//    result = QString("{\n"
//            "  \"ifc\": \"rs485\",\n"
//            "  \"portname\": \"ttyS0\",\n"
//            "  \"baudrate\": 19200,\n"
//            "  \"databits\": 8,\n"
//            "  \"flowcontrol\": 0,\n"
//            "  \"parity\": 0,\n"
//            "  \"stopbits\": 2\n"
//            "}");

//    break;


//  case UDP:

//    result = QString("{ \"host\": \"192.168.1.1\", \"recv_port\": 5300, \"send_port\": 5800 }");

//    break;
//  }

//  return result;

//}

//VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces()
//{
//  QList<QString> result = ifcesMap.keys();
//  return result;
//}

//VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams()
//{
//  return QString("{\n"
//                 "  \"start_register\": \"0x0000\",\n"
//                 "  \"reset_timeout\": 20\n"
//                 "}");
//}

#include "conning_kongsber_device.h"


ConningKongsberDevice::ConningKongsberDevice(sv::SvAbstractLogger *logger):
  ad::SvAbstractDevice(logger)
{
  D.clear();
  A.clear();

  signals_by_type.clear();
  signals_by_type.insert("D", &D);
  signals_by_type.insert("A", &A);
}

ConningKongsberDevice::~ConningKongsberDevice()
{
  emit stopThread();
  deleteLater();
}

bool ConningKongsberDevice::configure(const ad::DeviceInfo& info)
{
  p_info = info;

  try {

    /* парсим - проверяем, что парметры устройства заданы верно */
    DeviceParams::fromJson(p_info.dev_params);

    /* парсим - проверяем, что парметры для указанного интерфейса заданы верно */
    switch (ifcesMap.value(p_info.ifc_name.toUpper(), AvailableIfces::Undefined)) {

      case AvailableIfces::RS485:
      case AvailableIfces::RS:

        SerialParams::fromJsonString(p_info.ifc_params);

        break;

      case AvailableIfces::UDP:

        UdpParams::fromJsonString(p_info.ifc_params);

        break;

      case AvailableIfces::TEST:
        TestParams::fromJsonString(p_info.ifc_params);
        break;

      default:
        p_exception.raise(QString("Неизвестный тип интерфейса: %1").arg(p_info.ifc_name));
        break;
    }

    is_configured = true;

    return true;

  }

  catch(SvException& e) {

    is_configured = false;

    setLastError(e.error);
    return false;
  }
}

void ConningKongsberDevice::addSignal(SvSignal* signal) throw(SvException)
{
  ad::SvAbstractDevice::addSignal(signal);

  ckng::SignalParams p = ckng::SignalParams::fromSignal(signal);
  if(signal->info()->type == "D")
  {
   if(D.contains(p.group * 16 + p.bit))
     throw SvException(QString(S_IMPERMISSIBLE_VALUE)
                       .arg(QString("(%1, %2)").arg(P_GROUP).arg(P_BIT))
                       .arg(QString("%1, %2").arg(p.group).arg(p.bit))
                       .arg(QString("Сигнал %1: значение сдвига %1*16+%2=%3 уже задано для другого сигнала")
                            .arg(signal->info()->name).arg(P_GROUP).arg(P_BIT).arg(p.group*16+p.bit)));

    D.insert(p.group * 16 + p.bit, signal->info()->name);

  }
  else if(signal->info()->type == "A")
  {
    if(A.contains(p.group * 16 + p.bit))
      throw SvException(QString(S_IMPERMISSIBLE_VALUE)
                        .arg(QString("(%1, %2)").arg(P_GROUP).arg(P_BIT))
                        .arg(QString("%1, %2").arg(p.group).arg(p.bit))
                        .arg(QString("Сигнал %1: значение сдвига %1*16+%2=%3 уже задано для другого сигнала")
                             .arg(signal->info()->name).arg(P_GROUP).arg(P_BIT).arg(p.group*16+p.bit)));

    A.insert(p.group * 16 + p.bit, signal->info()->name);

  }
}

bool ConningKongsberDevice::open()
{
  try {

    if(!is_configured)
      p_exception.raise(QString("Для устройства '%1' не задана конфигурация").arg(p_info.name));

    create_new_thread();

    p_thread->conform(p_info.dev_params, p_info.ifc_params);

    connect(p_thread, &ad::SvAbstractDeviceThread::finished, this, &ConningKongsberDevice::deleteThread);
    connect(this, &ConningKongsberDevice::stopThread, p_thread, &ad::SvAbstractDeviceThread::stop);

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

void ConningKongsberDevice::create_new_thread() throw(SvException)
{
  try {

    switch (ifcesMap.value(p_info.ifc_name.toUpper(), AvailableIfces::Undefined)) {

      case AvailableIfces::RS485:
      case AvailableIfces::RS:

        p_thread = new ConningKongsberSerialThread(this, p_logger);

        break;

      case AvailableIfces::UDP:

        p_thread = new ConningKongsberUDPThread(this, p_logger);
        break;


      case AvailableIfces::TEST:
        p_thread = new ConningKongsberTestThread(this, p_logger);
        break;

    default:
      p_exception.raise(QString("Неизвестный тип интерфейса: %1").arg(info()->ifc_name));
      break;

    }

    static_cast<ConningKongsberGenericThread*>(p_thread)->setSignalsMap(&signals_by_type);

  }

  catch(SvException& e) {

    throw e;

  }
}

void ConningKongsberDevice::close()
{
  emit stopThread();

  p_isOpened = false;
}

void ConningKongsberDevice::deleteThread()
{
  if(p_thread)
    delete p_thread;
}


/** ******* UDP THREAD ************* **/
ConningKongsberUDPThread::ConningKongsberUDPThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger):
  ConningKongsberGenericThread(device, logger)
{

}

ConningKongsberUDPThread::~ConningKongsberUDPThread()
{
  deleteLater();
}

void ConningKongsberUDPThread::conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException)
{
  try {

    dev_params = DeviceParams::fromJson(jsonDevParams);
    ifc_params = UdpParams::fromJsonString(jsonIfcParams);

  }
  catch(SvException& e) {

    throw e;
  }
}

void ConningKongsberUDPThread::open() throw(SvException)
{
  if(!socket.bind(ifc_params.host, ifc_params.recv_port, QAbstractSocket::DontShareAddress))
    throw p_exception.assign(socket.errorString());

  // с заданным интервалом сбрасываем буфер, чтобы отсекать мусор и битые пакеты
  p_reset_timer.setInterval(dev_params.reset_timeout);
  p_reset_timer.setSingleShot(true);

  connect(&socket, SIGNAL(readyRead()), &p_reset_timer, SLOT(start()));
  connect(&p_reset_timer, &QTimer::timeout, this, &ad::SvAbstractDeviceThread::reset_buffer);

  // именно после open!
  socket.moveToThread(this);
}

void ConningKongsberUDPThread::run()
{
  p_is_active = true;

  while(p_is_active) {

    while(socket.waitForReadyRead(1) && p_is_active) {

      while(socket.hasPendingDatagrams() && p_is_active)
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

quint64 ConningKongsberUDPThread::write(const QByteArray& data)
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
  quint64 w = socket.writeDatagram(data, ifc_params.host, ifc_params.send_port);
  socket.flush();

  return w;
}

void ConningKongsberUDPThread::stop()
{
  p_is_active = false;
}


/** ******* Serial THREAD ************* **/
ConningKongsberSerialThread::ConningKongsberSerialThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger):
  ConningKongsberGenericThread(device, logger)
{

}

ConningKongsberSerialThread::~ConningKongsberSerialThread()
{
  deleteLater();
}

void ConningKongsberSerialThread::conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException)
{
  try {

    dev_params = DeviceParams::fromJson(jsonDevParams);
    ifc_params = SerialParams::fromJsonString(jsonIfcParams);

  }
  catch(SvException& e) {

    throw e;
  }
}

void ConningKongsberSerialThread::open() throw(SvException)
{
  port.setPortName   (ifc_params.portname   );
  port.setBaudRate   (ifc_params.baudrate   );
  port.setStopBits   (ifc_params.stopbits   );
  port.setFlowControl(ifc_params.flowcontrol);
  port.setDataBits   (ifc_params.databits   );
  port.setParity     (ifc_params.parity     );

  if(!port.open(QIODevice::ReadWrite))
    throw p_exception.assign(port.errorString());

  // с заданным интервалом сбрасываем буфер, чтобы отсекать мусор и битые пакеты
  p_reset_timer.setInterval(dev_params.reset_timeout);

  connect(&port, SIGNAL(readyRead()), &p_reset_timer, SLOT(start()));
  connect(&p_reset_timer, &QTimer::timeout, this, &ad::SvAbstractDeviceThread::reset_buffer);

  // именно после open!
  port.moveToThread(this);

}

void ConningKongsberSerialThread::run()
{
  p_is_active = true;

  while(p_is_active) {

    while(port.waitForReadyRead(1) && p_is_active) {

      if(p_buff.offset > MAX_PACKET_SIZE)
        reset_buffer();

      p_buff.offset += port.read((char*)(&p_buff.buf[p_buff.offset]), MAX_PACKET_SIZE - p_buff.offset);

      process_data();

    }
  }

  port.close();

}

quint64 ConningKongsberSerialThread::write(const QByteArray& data)
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

void ConningKongsberSerialThread::stop()
{
  p_is_active = false;
}


/** ******* Test THREAD ************* **/

ConningKongsberTestThread::ConningKongsberTestThread(ad::SvAbstractDevice *device, sv::SvAbstractLogger* logger):
  ConningKongsberGenericThread(device, logger)
{

}

ConningKongsberTestThread::~ConningKongsberTestThread()
{
  deleteLater();
}

void ConningKongsberTestThread::conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException)
{
  try {

    dev_params = DeviceParams::fromJson(jsonDevParams);
    ifc_params = TestParams::fromJsonString(jsonIfcParams);

  }
  catch(SvException& e) {
    throw e;
  }
}

void ConningKongsberTestThread::open() throw(SvException)
{
  *p_logger << sv::log::mtSuccess << QString("%1 открыт").arg(p_device->info()->name) << sv::log::endl;
}

quint64 ConningKongsberTestThread::write(const QByteArray& data)
{
  Q_UNUSED(data);

  *p_logger << sv::log::mtSuccess << QString("%1 записал данные").arg(p_device->info()->name) << sv::log::endl;

  return 0;
}

void ConningKongsberTestThread::run()
{
  p_is_active = true;

  int ref = 1;
  while(p_is_active)
  {
    QString msg = QString("$IIXDR,%1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,1,0,7*22dd").arg(ref);

    memcpy(&p_buff.buf[0], msg.toStdString().c_str(), msg.length());
    p_buff.offset = msg.length();

    process_data();

//    if(ifc_params.show_time)
//      *p_logger << sv::log::mtInfo << sv::log::TimeZZZ << ifc_params.testmsg << ifc_params.testval << sv::log::endl;
//    else
//      *p_logger << sv::log::mtInfo << ifc_params.testmsg << ifc_params.testval << sv::log::endl;

    msleep(ifc_params.period);
  }
}

void ConningKongsberTestThread::stop()
{
  p_is_active = false;
}


/** **** GENERIC FUNCTIONS **** **/
void ConningKongsberGenericThread::setSignalsMap(ckng::SignalsByTypeMap *sbt)
{
  signals_by_type = sbt;
//  return sv::log::sender::make(p_logger->options().log_sender_name_format,
//                               p_info.name,
//                               p_info.index);
}

void ConningKongsberGenericThread::process_data()
{
  QStringList l = QString::fromUtf8((const char*)(&p_buff.buf[0]), p_buff.offset).split("\r\n");

  for(QString current_message: l) //int i = 0; i < l.count(); ++i)
  {

    /* если в строке есть заголовок */
    if(current_message.startsWith(p_header))
    {
      /* и есть конец (контр. сумма), значит эта строка содержит полное сообщение. парсим */
      if(m_tail.match(current_message.right(3)).hasMatch())

        parseNlog(current_message);

      /* если есть заголовок, но нет 'контр. суммы', то запоминаем то что есть, как 'голову',
       * а следующее сообщение будем крепить к 'голове' как 'хвост' */
      else
        m_head = current_message;

    }

    /* если текущее сообщение не содержит заголовка, но содержит 'контр. сумму',
     * тогда, если 'голова' не пустая, крепим этот 'хвост' к 'голове' */
    else if(m_tail.match(current_message.right(3)).hasMatch())
    {
      if(!m_head.isEmpty())
      {
        current_message.push_front(m_head);

        parseNlog(current_message);

      }

      /* если 'голова' пустая, значит были пакеты с другим заголовком или
       * какие то пакеты были не полные (битые). не обрабатываем */

    }

    /* если в строке нет ни заголовка, ни конца, тогда, если 'голова' не пустая,
     * крепим текущее сообщение к голове. а иначе отбрасываем текущее сообщение как мусор */
    else
    {
      if(!m_head.isEmpty())
        m_head.append(current_message);

    }
  }

  reset_buffer();

}

void ConningKongsberGenericThread::parseNlog(QString message)
{
  m_head = "";

  if(!(message.startsWith(p_header) && m_tail.match(message.right(3)).hasMatch()))
    return;

  if(p_logger)
    *p_logger << sv::log::mtDebug << sv::log::llDebug << sv::log::TimeZZZ
              << sv::log::in << message << sv::log::endl;

  // если пакеты сыпятся (для данного получателя), то считаем, что линия передачи
  // в порядке и задаем новую контрольную точку времени для данного устройства
  p_device->setNewLostEpoch();

  // определяем код поступившего сообщения
  bool ok;
  QString type;

  int reference = message.mid(p_header.length(), 1).toInt(&ok);

  if(!ok) return;

  switch (reference) {
    case 1:
    case 2:
    case 3:
    case 4:
      type = "D";
      break;

    case 5:
    case 6:
    case 7:
      type = "A";
      break;

    default:
      return;
  }

  // проходим по списку полученных значений
  QList<QString> l = message.mid(p_header.length() + 2, message.indexOf(m_tail) - p_header.length() + 2).split(',');

  for(int i = 0; i < l.count(); ++i)
  {
    // находим сигнал
    ckng::SignalByOffsetMap* sbo = signals_by_type->value(type);

    if(!sbo->contains(reference * 16 + i))
      return;

    QString signal_name = sbo->value(reference * 16 + i);
    // определяем значение
    qreal r = l.at(i).trimmed().toDouble(&ok);

    if(!ok || l.at(i).trimmed().isEmpty())
      p_device->setSignalValue(signal_name, p_device->Signals()->value(signal_name)->info()->timeout_value);

    else
      p_device->setSignalValue(signal_name, r);

  }
}

QByteArray ConningKongsberGenericThread::confirmation()
{
  QByteArray confirm;
//  confirm.append((const char*)(&header), 6);

//  // вычисляем crc ответа
//  quint16 crc = CRC::MODBUS_CRC16((uchar*)(&header), 6);
//  confirm.append(quint8(crc & 0xFF));
//  confirm.append(quint8(crc >> 8));

  return confirm;

}


/** ********** EXPORT ************ **/
ad::SvAbstractDevice* create()
{
  ad::SvAbstractDevice* device = new ConningKongsberDevice();
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

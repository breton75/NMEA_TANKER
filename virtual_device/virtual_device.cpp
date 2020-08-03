#include "virtual_device.h"


VirtualDevice::VirtualDevice(sv::SvAbstractLogger *logger):
  ad::SvAbstractDevice(logger)
{

}

bool VirtualDevice::setup(const ad::DeviceInfo& info)
{
  p_info = info;

  try {

//    p_params = dev::DeviceParams::fromJson(p_info.device_params);

//    if(!p_params.isValid)
//      p_exception->raise(QString("Неверные параметры устройства: %1").arg(p_info.device_params));

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
      p_exception->raise(QString("Неизвестный тип интерфейса: %1").arg(info()->ifc_name));
      break;

    }
  }

  catch(SvException& e) {

    throw e;

  }
}


/** ******* UDP THREAD ************* **/
VirtualDeviceUDPThread::VirtualDeviceUDPThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger):
  ad::SvAbstractDeviceThread(device, logger)
{

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

void VirtualDeviceUDPThread::open() throw(SvException&)
{

}

void VirtualDeviceUDPThread::process_data()
{

}

void VirtualDeviceUDPThread::stop()
{

}


/** ******* Serial THREAD ************* **/
VirtualDeviceSerialThread::VirtualDeviceSerialThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger):
  ad::SvAbstractDeviceThread(device, logger)
{

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

void VirtualDeviceSerialThread::open() throw(SvException&)
{

}

void VirtualDeviceSerialThread::process_data()
{

}

void VirtualDeviceSerialThread::stop()
{

}


/** ********** EXPORT ************ **/
VirtualDevice* create()
{
  VirtualDevice* device = new VirtualDevice();
  return device;
}


QString defaultIfcParams(const QString& ifc = QString())
{
  QString result = "";

  switch(ifcesMap.value(ifc.toUpper()))
  {
    case RS:
    case RS485:

    result = QString("{\n"
            "  \"ifc\": \"rs485\",\n"
            "  \"portname\": \"ttyS0\",\n"
            "  \"baudrate\": 19200,\n"
            "  \"databits\": 8,\n"
            "  \"flowcontrol\": 0,\n"
            "  \"parity\": 0,\n"
            "  \"stopbits\": 2\n"
            "}");

    break;


  case UDP:

    result = QString("{ \"host\": \"192.168.1.1\", \"recv_port\": 5300, \"send_port\": 5800 }");

    break;
  }

  return result;

}

QList<QString> availableInterfaces()
{
  QList<QString> result = ifcesMap.keys();
  return result;
}

QString defaultDeviceParams()
{
  return QString("{\n"
                 "  \"start_register\": \"0x0000\",\n"
                 "  \"reset_timeout\": 20\n"
                 "}");
}

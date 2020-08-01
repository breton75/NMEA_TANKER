#include "virtual_device.h"


VirtualDevice::VirtualDevice(sv::SvAbstractLogger *logger):
  ad::SvAbstractDevice(logger)
{

}

VirtualDevice::~VirtualDevice()
{
  emit stopThread();
  deleteLater();
}

bool VirtualDevice::setup(const ad::DeviceInfo& info)
{
  p_info = info;
  return true;
}

void VirtualDevice::deleteThread()
{
  if(p_thread) delete p_thread;
}

bool VirtualDevice::open() throw(SvException)
{
  try {

    SvException e = create_new_thread();

    if(e.type != SvException::NoError)
      throw e;

    p_thread->setIfcParams(p_info.ifc_params);

    connect(p_thread, &ad::SvAbstractDeviceThread::finished, this, &VirtualDevice::deleteThread);
    connect(this, &VirtualDevice::stopThread, p_thread, &ad::SvAbstractDeviceThread::stop);
//    connect(p_thread, &dev::SvAbstractDeviceThread::finished, p_thread, &dev::SvAbstractDeviceThread::deleteLater);

    p_thread->open();
    p_thread->start();

    return true;

  } catch(SvException& e) {

    p_last_error = e.error;

    deleteThread();

    return false;

  }
}

void VirtualDevice::close()
{
  emit stopThread();
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

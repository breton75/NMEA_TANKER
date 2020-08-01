#include "sv_abstract_udp_device.h"

dev::SvAbstractUdpDevice::SvAbstractUdpDevice(dev::HardwareType type, sv::SvAbstractLogger *logger):
  dev::SvAbstractDevice(type, logger)
//  p_log(log)
{

}

dev::SvAbstractUdpDevice::~SvAbstractUdpDevice()
{
  deleteThread();
  deleteLater();
}

bool dev::SvAbstractUdpDevice::setConfig(const dev::DeviceConfig& config)
{
  p_config = config;
  return true;
}

bool dev::SvAbstractUdpDevice::setParams(const QString& params)
{
  try {

    dev::DeviceParamsParser p(params);

    if(!p.parse())
      p_exception->raise(p.lastError());

    p_params = p.params();

    return true;

  }

  catch(SvException& e) {

    setLastError(e.error);
    return false;
  }
}

bool dev::SvAbstractUdpDevice::open()
{
  try {

    create_new_thread();

    connect(p_thread, &dev::SvAbstractDeviceThread::finished, this, &dev::SvAbstractUdpDevice::deleteThread);
    connect(p_thread, &dev::SvAbstractDeviceThread::finished, p_thread, &dev::SvAbstractDeviceThread::deleteLater);

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

void dev::SvAbstractUdpDevice::close()
{
  deleteThread();

  p_isOpened = false;
}

void dev::SvAbstractUdpDevice::deleteThread()
{
  if(p_thread) {

    delete p_thread;
    p_thread = nullptr;

  }
}


/**         SvAbstractUdpDeviceThread         **/
dev::SvAbstractUdpDeviceThread::SvAbstractUdpDeviceThread(dev::SvAbstractDevice *device, sv::SvAbstractLogger *logger):
  dev::SvAbstractDeviceThread(logger),
  p_device(device),
  is_active(false)
{

}

dev::SvAbstractUdpDeviceThread::~SvAbstractUdpDeviceThread()
{
  stop();
}

void dev::SvAbstractUdpDeviceThread::stop()
{
  is_active = false;
  while(this->isRunning()) qApp->processEvents();
}

void dev::SvAbstractUdpDeviceThread::open() throw(SvException&)
{
  p_socket.bind(p_device->params()->udp_port);
  p_port.setBaudRate   (p_device->params()->serialParams.baudrate);
  p_port.setStopBits   (p_device->params()->serialParams.stopbits);
  p_port.setFlowControl(p_device->params()->serialParams.flowcontrol);
  p_port.setDataBits   (p_device->params()->serialParams.databits);
  p_port.setParity     (p_device->params()->serialParams.parity);

  if(!p_port.open(QIODevice::ReadWrite))
    throw p_exception.assign(p_port.errorString());

  // с заданным интервалом сбрасываем буфер, чтобы отсекать мусор и битые пакеты
  p_reset_timer.setInterval(RESET_INTERVAL);
  connect(&p_port, SIGNAL(readyRead()), &p_reset_timer, SLOT(start()));
  connect(&p_reset_timer, &QTimer::timeout, this, &dev::SvAbstractUdpDeviceThread::reset_buffer);

  // именно после open!
  p_port.moveToThread(this);

}

void dev::SvAbstractUdpDeviceThread::reset_buffer()
{
    p_buf_offset = 0;
}

void dev::SvAbstractUdpDeviceThread::run()
{
  is_active = true;

  while(is_active) {

    while(p_port.waitForReadyRead(1)) {

      if(p_buf_offset > 512)
        reset_buffer();

      p_bytes_readed = p_port.read((char*)(&p_buf[0] + p_buf_offset), 512 - p_buf_offset);

      p_buf_offset += p_bytes_readed;


      treat_data();


    }
  }

  p_socket.close();

}


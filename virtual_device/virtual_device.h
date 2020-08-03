#ifndef VIRTUAL_DEVICE_H
#define VIRTUAL_DEVICE_H

#include "virtual_device_global.h"

#include "../global/sv_abstract_generic_device.h"
#include "../global/sv_signal.h"

#include "../../svlib/sv_abstract_logger.h"
#include "../../svlib/sv_exception.h"

#include "serial_params.h"
#include "udp_params.h"

enum AvailableIfces {
  RS,
  RS485,
  UDP
};

const QMap<QString, AvailableIfces> ifcesMap = {{"RS",    AvailableIfces::RS},
                                                {"RS485", AvailableIfces::RS485},
                                                {"UDP",   AvailableIfces::UDP}};


class VirtualDevice: public SvAbstractGenericDevice
{
  Q_OBJECT

public:
  VirtualDevice(sv::SvAbstractLogger* logger = nullptr);
  ~VirtualDevice();

  void create_new_thread() throw(SvException);

  bool setup(const ad::DeviceInfo& info);

  bool open();
  void close();

private slots:
  void deleteThread();

signals:
  void stopThread();

};

class VirtualDeviceUDPThread: public SvAbstractUdpThread
{

public:
  VirtualDeviceUDPThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr);


  void setIfcParams(const QString& params) throw(SvException&);
  void open() throw(SvException&);

private:
  UdpParams ifc_params;

  void process_data();


public slots:
  void stop();

};

class VirtualDeviceSerialThread: public ad::SvAbstractDeviceThread
{

public:
  VirtualDeviceSerialThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr);

  void setIfcParams(const QString& params) throw(SvException&);
  void open() throw(SvException&);

private:
  SerialParams ifc_params;

  void process_data();


public slots:
  void stop();

};


extern "C" {

//    enum {
//        ERROR = -1,
//        OK = 0
//    };

//  class VirtualDevice;
//  class VirtualDeviceThread;
//    struct VIRTUAL_DEVICESHARED_EXPORT can_err{
//        int code = canlib::OK;
//        char *msg = nullptr;
//    };

//    struct VIRTUAL_DEVICESHARED_EXPORT can_params {
//        unsigned int error_filter = 0;
//        int loopback = -1;       // -1 - unssigned, 0 - false, any other - true
//        int receive_own = -1;    // -1 - unssigned, 0 - false, any other - true
//        int can_fd = -1;         // -1 - unssigned, 0 - false, any other - true
//        unsigned int bitrate = 0;  // 0 - unssigned

//        int extended_frame_format = 0; // 0 - false (id is 11 bits number), any other - true (id is 29 bits number)
//        int flexible_rate_format = 0;  // 0 - false (max 8 byte per frame), any other - true (max 64 byte per frame)
//    };

//    struct CANLIBSHARED_EXPORT can_frame {
//      char *device;
//      unsigned int id;
//      int type;
//      char *data;
//      unsigned int data_length;
//      char *data_str;
//      unsigned int microseconds;
//    };

//    CANLIBSHARED_EXPORT can_err *get_can_list();

    VIRTUAL_DEVICESHARED_EXPORT VirtualDevice* create();

    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();


//    VIRTUAL_DEVICESHARED_EXPORT SvException setParams(dev::SvAbstractDevice* device, const QString params);
//    VIRTUAL_DEVICESHARED_EXPORT SvException setIfc(dev::SvAbstractDevice* device, const QString ifc);

//    VIRTUAL_DEVICESHARED_EXPORT SvException addSignal(SvSignal* signal);


}





#endif // VIRTUAL_DEVICE_H

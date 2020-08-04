#ifndef VIRTUAL_DEVICE_H
#define VIRTUAL_DEVICE_H

#include "virtual_device_global.h"

#include "../global/sv_abstract_device.h"
#include "../global/sv_signal.h"

#include "../../svlib/sv_abstract_logger.h"
#include "../../svlib/sv_exception.h"
#include "../../svlib/sv_crc.h"

#include "serial_params.h"
#include "udp_params.h"
#include "device_params.h"

namespace vir {

  #pragma pack(push,1)
  struct Header
  {
    quint8  client_addr;
    quint8  func_code;
    quint8  ADDRESS;
    quint8  OFFSET;
    quint16 register_count;
    quint8  byte_count;
  };
  #pragma pack(pop)

//  bool process_data(ad::SvAbstractDevice* device, ad::BUFF* buff, ad::DATA* data, Header* header);

}

enum AvailableIfces {
  RS,
  RS485,
  UDP
};

const QMap<QString, AvailableIfces> ifcesMap = {{"RS",    AvailableIfces::RS},
                                                {"RS485", AvailableIfces::RS485},
                                                {"UDP",   AvailableIfces::UDP}};

class VirtualDevice: public ad::SvAbstractDevice
{
  Q_OBJECT

public:
  VirtualDevice(sv::SvAbstractLogger* logger = nullptr);
  ~VirtualDevice();

  bool setup(const ad::DeviceInfo& info);

  bool open();
  void close();

  void create_new_thread() throw(SvException);

  DeviceParams* params() { return &_params; }

private:
  DeviceParams _params;

private slots:
  void deleteThread();

signals:
  void stopThread();

};

class VirtualDeviceGenericThread: public ad::SvAbstractDeviceThread
{
  Q_OBJECT

public:
  VirtualDeviceGenericThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr):
    ad::SvAbstractDeviceThread(device, logger)
  {   }

protected:
  DeviceParams* dev_params;

  vir::Header header;
  size_t hsz = sizeof(vir::Header);

  quint8  confirm[8];

  void process_data();

private:
  quint16 parse_data();
  QByteArray confirmation();

  void func_virtual();

};

class VirtualDeviceUDPThread: public VirtualDeviceGenericThread
{
  Q_OBJECT

public:
  VirtualDeviceUDPThread(ad::SvAbstractDevice *device, sv::SvAbstractLogger* logger = nullptr);
  ~VirtualDeviceUDPThread();

  void setIfcParams(const QString& params) throw(SvException&);

  void open() throw(SvException);

  quint64 write(const QByteArray& data);

private:
  QUdpSocket socket;

  UdpParams     ifc_params;


  void run() Q_DECL_OVERRIDE;

public slots:
  void stop();

};

class VirtualDeviceSerialThread: public VirtualDeviceGenericThread
{
  Q_OBJECT

public:
  VirtualDeviceSerialThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr);
  ~VirtualDeviceSerialThread();

  void setIfcParams(const QString& params) throw(SvException&);

  void open() throw(SvException);

  quint64 write(const QByteArray& data);

private:
  QSerialPort port;

  SerialParams  ifc_params;
  DeviceParams* dev_params;

  void run() Q_DECL_OVERRIDE;

public slots:
  void stop();

};


extern "C" {

//    enum {
//        ERROR = -1,
//        OK = 0
//    };

//   class VIRTUAL_DEVICESHARED_EXPORT VirtualDevice;
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

    VIRTUAL_DEVICESHARED_EXPORT void* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();


//    VIRTUAL_DEVICESHARED_EXPORT SvException setParams(dev::SvAbstractDevice* device, const QString params);
//    VIRTUAL_DEVICESHARED_EXPORT SvException setIfc(dev::SvAbstractDevice* device, const QString ifc);

//    VIRTUAL_DEVICESHARED_EXPORT SvException addSignal(SvSignal* signal);


}





#endif // VIRTUAL_DEVICE_H

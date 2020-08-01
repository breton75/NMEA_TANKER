#ifndef SV_IDEVICE_H
#define SV_IDEVICE_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QDateTime>
#include <QMetaType>
#include <QtSerialPort/QSerialPort>
#include <QDebug>

#include "sv_signal.h"
#include "device_params.h"

namespace idev {

  enum HardwareTypes {

    sdtUndefined = -1,
    sdtOHT = 40,
    sdtOPA = 20,
//    sdtOPA_SS1_119 = 20,
//    sdtOPA_SS1_122 = 21,
//    sdtOPA_12SS1_218 = 22,
//    sdtOPA_12SS1_8 = 23,
//    sdtOPA_12SS1_38 = 24,
//    sdtOPA_12SS1_67 = 25,
//    sdtOPA_12SS1_93 = 26,
//    sdtOPA_12SS1_123 = 27,
//    sdtOPA_12SS1_150 = 28,
    sdtSKM = 39,
    sdtKTV = 99
  };

  const QMap<QString, HardwareTypes> HARDWARE_CODES = {{"OHT", sdtOHT},
                                                       {"OPA", sdtOPA},
                                                       {"SKM", sdtSKM},
                                                       {"KTV", sdtKTV}};

//  const QMap<HardwareTypes, int> OPA_REGISTERS = {{sdtOPA_SS1_119,   0x0400},
//                                                {sdtOPA_SS1_122,   0x04A0},
//                                                {sdtOPA_12SS1_218, 0x0540},
//                                                {sdtOPA_12SS1_8,   0x05E0},
//                                                {sdtOPA_12SS1_38,  0x0680},
//                                                {sdtOPA_12SS1_67,  0x0720},
//                                                {sdtOPA_12SS1_93,  0x07C0},
//                                                {sdtOPA_12SS1_123, 0x0860},
//                                                {sdtOPA_12SS1_150, 0x0900}};

  struct DeviceConfig
  {
//    int id = -1;
    int index = -1;
    QString name = "";
    HardwareTypes hardware_type = HardwareTypes::sdtUndefined;
    int ifc_id = -1;
    QString ifc_name = "";
    int protocol_id = -1;
    QString protocol_name = "";
    QString driver_lib_name = "";
    QString description = "";
    bool is_involved = false;
    bool debug_mode = false;
    bool debug2_mode = false;
    quint64 timeout = 0;
    QString device_params_string = "";
    
//    DeviceConfig &operator= (DeviceConfig &other) {
      
////      id = other.id;
//      name = other.name;
//      index = other.index;
//      ifc_id = other.ifc_id;
//      ifc_name = other.ifc_name;
//      protocol_id = other.protocol_id;
//      protocol_name = other.protocol_name;
//      driver_lib_name = other.driver_lib_name;
//      description = other.description;
//      is_configured = other.is_configured;
//    }
    
  };
  
  class SvIDevice;

}


/** ----------- SvIDevice ------------ **/
class idev::SvIDevice : public QObject
{
    Q_OBJECT
    
public:
  SvIDevice(const idev::HardwareTypes hardware_type):
    p_hardware_type(hardware_type)
  { clearSignals(); }
  
  virtual ~SvIDevice() { }
  
  virtual idev::HardwareTypes hardware_type() const { return p_hardware_type; }
  

  void setLastError(const QString& lastError) { p_last_error = lastError; }
  QString lastError()                         { return p_last_error; }
  
//  void setDeviceType(idev::DeviceTypes type) { p_type = type; }
//  idev::DeviceTypes deviceType()             { return p_type; }

  void setOpened(bool isOpened) { p_isOpened = isOpened; }
  bool isOpened() const         { return p_isOpened; }
  
  void setActive(bool isActive) { p_isActive = isActive; }
  bool isActive() const         { return p_isActive; }
  
//  void setTimeout(quint32 timeout) { _timeout = timeout; }
  void setNewLostEpoch()
  {
      p_lost_epoch = QDateTime::currentMSecsSinceEpoch() + p_config.timeout;
//        qDebug() << "timeout:" << _config.timeout << " device:" << _config.name << " lost_epoch:" << _lost_epoch;
      foreach (SvSignal* s, p_signals.values())
        s->setDeviceLostEpoch(p_lost_epoch);
  }

  bool isAlive() { return p_lost_epoch > quint64(QDateTime::currentMSecsSinceEpoch());  }

  virtual const idev::DeviceConfig* config() const { return &p_config; }
  virtual bool setConfig(const idev::DeviceConfig& config) { p_config = config; return true; }
  
  virtual const dev::DeviceParams* params() const { return &p_device_params; }
  virtual bool setParams(const QString& params)  = 0; //{ _params = params; }
  
//  virtual void write(const QByteArray* data) = 0;
//  virtual void read() = 0;
  
  void addSignal(SvSignal* signal) { p_signals.insert(signal->params()->name, signal); }
  void clearSignals() { p_signals.clear(); }
  const QMap<QString, SvSignal*>* Signals() const { return &p_signals; }
  int signalsCount() { return p_signals.count(); }
  
  void setSignalValue(const QString SIGNAL_NAME, qreal VALUE)
  {
    if(p_signals.contains(SIGNAL_NAME)) {

//      qDebug() << QString("SIGNAL_NAME: %1   VALUE: %2").arg(SIGNAL_NAME).arg(VALUE);
      p_signals.value(SIGNAL_NAME)->setValue(VALUE);

    }

  }
  
  
//  SvSignal* firstSignal()
//  { 
//    if(_signals.count()) {
//      _pos = 0;
//      return _signals[_pos];
//    }
//    else return Q_NULLPTR;
//  }
  
//  SvSignal* nextSignal()
//  {
//    return _pos < _signals.count() - 1 ? _signals[++_pos] : Q_NULLPTR;
//  }
    
protected:
  idev::DeviceConfig p_config;
  dev::DeviceParams  p_device_params;
  
  QMap<QString, SvSignal*> p_signals;
  
  int _pos = -1;

  quint64 p_lost_epoch = 0;

  quint32 _id;
  idev::HardwareTypes p_hardware_type;
  
  QString p_last_error;
  
  bool p_isOpened = false;
  bool p_isActive = true;
  bool p_is_ready_read = false;
  

public:
  virtual bool open() = 0;
  virtual void close() = 0;
  
//  virtual bool start(quint32 msecs) = 0; //{ Q_UNUSED(msecs); }
  virtual void stop() { }
  
    
//signals:
//  void new_data(const svidev::mdata_t& data);
      
};

#endif // SV_IDEVICE_H

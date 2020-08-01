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

namespace idev {

  enum DeviceTypes {
    sdtUndefined = -1,
    sdtOHT_Gamma12700 = 40,
    sdtOPA_SS1_119 = 20,
    sdtOPA_SS1_122 = 21,
    sdtOPA_12SS1_218 = 22,
    sdtOPA_12SS1_8 = 23,
    sdtOPA_12SS1_38 = 24,
    sdtOPA_12SS1_67 = 25,
    sdtOPA_12SS1_93 = 26,
    sdtOPA_12SS1_123 = 27,
    sdtOPA_12SS1_150 = 28,
    sdtSKM = 39,
    sdtSKTV = 99
  };

  const QMap<DeviceTypes, int> OPA_REGISTERS = {{sdtOPA_SS1_119,   0x0400},
                                                {sdtOPA_SS1_122,   0x04A0},
                                                {sdtOPA_12SS1_218, 0x0540},
                                                {sdtOPA_12SS1_8,   0x05E0},
                                                {sdtOPA_12SS1_38,  0x0680},
                                                {sdtOPA_12SS1_67,  0x0720},
                                                {sdtOPA_12SS1_93,  0x07C0},
                                                {sdtOPA_12SS1_123, 0x0860},
                                                {sdtOPA_12SS1_150, 0x0900}};

  struct DeviceConfig
  {
//    int id = -1;
    int index = -1;
    QString name = "";
    int ifc_id = -1;
    QString ifc_name = "";
    int protocol_id = -1;
    QString protocol_name = "";
    QString driver_lib_name = "";
    QString description = "";
    bool is_configured = false;
    bool debug_mode = false;
    int DEVICE_REGISTER = 0;
    quint64 timeout = 0;
    
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
  SvIDevice() { clearSignals(); }
  
  virtual ~SvIDevice() { }
  
  virtual idev::DeviceTypes type() const { return idev::sdtUndefined; }
  

  void setLastError(const QString& lastError) { _last_error = lastError; }
  QString lastError() { return _last_error; }
  
  void setDeviceType(idev::DeviceTypes type) { _type = type; }
  idev::DeviceTypes deviceType() { return _type; }
  
  void setOpened(bool isOpened) { _isOpened = isOpened; }
  bool isOpened() { return _isOpened; }
  
  void setActive(bool isActive) { _isActive = isActive; }
  bool isActive() { return _isActive; }
  
//  void setTimeout(quint32 timeout) { _timeout = timeout; }
  void setNewLostEpoch()
  {
      _lost_epoch = QDateTime::currentMSecsSinceEpoch() + _config.timeout;
//        qDebug() << "timeout:" << _config.timeout << " device:" << _config.name << " lost_epoch:" << _lost_epoch;
      foreach (SvSignal* s, _signals.values())
        s->setDeviceLosEpoch(_lost_epoch);
  }

  bool isAlive() { return _lost_epoch > quint64(QDateTime::currentMSecsSinceEpoch());  }

  bool isReadyRead() { return _is_ready_read; }
  
  virtual idev::DeviceConfig* config() { return &_config; }
  virtual bool setConfig(idev::DeviceConfig& config) { _config = config; return true; }
  
  virtual QString params() { return _params; }
  virtual bool setParams(const QString& params)  = 0; //{ _params = params; }
  
  virtual void write(const QByteArray* data) = 0;
  virtual void read() = 0;
  
  void addSignal(SvSignal* signal) { _signals.insert(signal->params()->name, signal); }
  void clearSignals() { _signals.clear(); }
  QMap<QString, SvSignal*>* Signals() { return &_signals; }
  int signalsCount() { return _signals.count(); }
  
  void setSignalValue(const QString SIGNAL_NAME, qreal VALUE)
  {
    if(_signals.contains(SIGNAL_NAME)) {

//      qDebug() << QString("SIGNAL_NAME: %1   VALUE: %2").arg(SIGNAL_NAME).arg(VALUE);
      _signals.value(SIGNAL_NAME)->setValue(VALUE);

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
  idev::DeviceConfig _config;
  
  QMap<QString, SvSignal*> _signals;
  
  int _pos = -1;

  quint64 _lost_epoch = 0;

  quint32 _id;
  idev::DeviceTypes _type;
  
  QString _last_error;
  
  bool _isOpened = false;
  bool _isActive = true;
  bool _is_ready_read = false;
  
  QString _params;

public:
  virtual bool open() = 0;
  virtual void close() = 0;
  
//  virtual bool start(quint32 msecs) = 0; //{ Q_UNUSED(msecs); }
  virtual void stop() { }
  
    
//signals:
//  void new_data(const svidev::mdata_t& data);
      
};

#endif // SV_IDEVICE_H

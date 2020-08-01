#ifndef SV_SIGNAL_H
#define SV_SIGNAL_H

#include <QObject>
#include <QDateTime>
#include <QVariant>
#include <QTimer>
#include <QList>
#include <QDebug>


enum SignalDataTypes {
  dtInt = 0,
  dtFloat
};

struct SignalParams
{
  SignalParams(SignalDataTypes dataType = dtInt) { data_type = dataType; }
  
  int     index = -1;
  qint64  cob_id = -1;
  QString name = "";
  quint32 data_offset = 0;
  quint32 data_length = 0;
  QString description = "";
  quint32 timeout = 3000;
  int timeout_value = -3;
  int timeout_signal_index = -1;
  SignalDataTypes data_type = dtInt;
  bool is_involved = false;
  bool storage0_linked = false;
  bool storage1_linked = false;
  bool storage2_linked = false;
  
  int device_index = -1;
  QString device_name = "";
  
};

class SvSignal: public QObject
{
  Q_OBJECT
  
public:
  explicit SvSignal(SignalParams& params);
  ~SvSignal();
  
  int index() { return _index; }
  
  void setParams(SignalParams& params) { _params = params; _index = params.index; }
  const SignalParams* params() { return &_params; }
  
  quint64 lostEpoch() { return _lost_epoch; }
//  const QDateTime* lastUpdate()  { return &_last_update; }
  qreal value() { return _value; }// _params.data_type == dtInt ? _value.toInt() : _value.toFloat();  }
  bool isAlive() { return _lost_epoch > quint64(QDateTime::currentMSecsSinceEpoch()); }

  void setDeviceLostEpoch(const quint64 lost_epoch) { _device_lost_epoch = lost_epoch; }
  bool isDeviceAlive() { return _device_lost_epoch > quint64(QDateTime::currentMSecsSinceEpoch()); }

  bool operator==(SvSignal& other) const
  { 
    return _index == other.index();
  }
  
  bool operator==(SvSignal* other) const
  { 
    return _index == other->index();
  }

  qreal previousValue() const { return _previous_value; }

  bool setLostValue()
  {
    if(_value != _params.timeout_value) {

      _previous_value = _value;
      _value = _params.timeout_value;
      return true;
    }

    return false;

  }
  
private:
  SignalParams _params;
  
  int _index;
  
  quint64 _lost_epoch = 0;
  quint64 _device_lost_epoch = 0;

//  QDateTime _last_update;
//  bool _is_lost = false;
  qreal _value = 0.0; //QVariant();
  
  QTimer _timer;

  qreal _previous_value = 0.0;
  
public slots:
  void setValue(qreal value);
  
  
};

#endif // SV_SIGNAL_H

#ifndef SV_STORAGE_H
#define SV_STORAGE_H

#include <QObject>
#include <QThread>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QtMath>
#include <QMap>
#include <QList>

#include "../../svlib/sv_pgdb.h"
#include "../../svlib/sv_abstract_logger.h"
#include "../../svlib/sv_exception.h"

#include "../global/sql_defs.h"
#include "../global/sv_signal.h"

struct StorageParams
{
  int index = -1;
  QString name = "";
  QString host = "";
  quint32 port = 5432;
  QString login = "";
  QString pass = "";
  QString database_name = "";
  QString table_name = "";
  QString alter_host = "";
  quint32 alter_port = 5432;
};

//class SvCOB: public QObject
//{
//  Q_OBJECT
  
//public:
//  SvCOB(int cobid) { _id = cobid; }
//  ~SvCOB() { deleteLater(); } 
  
//  int id() { return _id; }
  
//  qint64 value() { return qint64(_value); }
//  void setSignalValue(SvSignal* signal)
//  {
//    // формируем маску данных сигнала
//    quint64 mask = quint64(qPow(2, signal->params()->data_length) - 1) << (63 - signal->params()->data_offset);
    
//    _value &= ~mask; // выключаем нужные биты
    
////    qDebug() << "mask" << QString::number(mask, 16) << "~mask" << QString::number(~mask, 16) << "_value" << _value << "signal" << signal->value() << signal->params()->data_length << signal->params()->data_offset;
//    _value |= signal->value() << (63 - signal->params()->data_offset); // вставляем новое значение
////    qDebug() << "_value" << _value;
//  }
  
//  void addSignal(SvSignal* signal) { _signals.append(signal); }
//  void clearSignals() { _signals.clear(); }
//  QVector<SvSignal*>* Signals() { return &_signals; }
//  int signalsCount() { return _signals.count(); }

//  SvSignal* firstSignal()
//  { 
//    if(_signals.count()) {
//      _pos = 0;
//      return _signals[_pos];
//    }
//    else return nullptr;
//  }
  
//  SvSignal* nextSignal()
//  {
//    return _pos < _signals.count() - 1 ? _signals[++_pos] : nullptr;
//  }
  
////  QMap<int, SvSignal*>::iterator end() { return _signals.end(); }
  
//private:
//  int _id;
  
//  QVector<SvSignal*> _signals;
//  int _pos = -1;
  
//  quint64 _value = 0;
  
//};


//typedef QMap<int, SvCOB*> COBMap;

class SvStorageThread;

class SvStorage: public QObject
{
    Q_OBJECT
  
public:
  SvStorage(StorageParams params, sv::SvAbstractLogger &log, QObject *parent = Q_NULLPTR);
  ~SvStorage();
  
  int index() { return _params.index; }
  
  bool init();
  void start();
  void stop();
  
  QString lastError() { return _last_error; }
  StorageParams* params() { return &_params; }
  
  void addSignal(SvSignal* signal) { _signals.append(signal); }
  void clearSignals() { _signals.clear(); }
  QList<SvSignal*>* Signals() { return &_signals; }
  int signalsCount() { return _signals.count(); }

  
private:
  StorageParams _params;
  QString _last_error = "";
  
  QList<SvSignal*> _signals;
  
  SvStorageThread* _thr = nullptr;
  
  SvException _exception;
  
  int _pos = -1;
  
  sv::SvAbstractLogger& _log;

  QTimer* _reconnect_timer = nullptr;

private slots:
  void reconnect();
  void start_reconnect_timer();
  void stop_reconnect_timer();

//  void stopThread();
  void deleteThread();

  void logerr(QString e);
  void logreconnect();
  
};
  

class SvStorageThread: public QThread
{
    Q_OBJECT
  
public:
  SvStorageThread(StorageParams *params, QList<SvSignal*>* signalList, QObject *parent = Q_NULLPTR);
  ~SvStorageThread();
  
  bool init();
  
  void run() Q_DECL_OVERRIDE;
  void stop();
  
  QString lastError() { return _last_error; }
  
private:
  StorageParams* _params;
  QList<SvSignal*>* _signals;
  
  SvPGDB* PGDB = nullptr;
  
  QString _last_error = "";
  
  bool _started = false;
  bool _finished = true;
  
  SvException _exception;
  
  int _pos;
  
  SvSignal* firstSignal()
  { 
    if(_signals->count()) {
      _pos = 0;
      return _signals->value(_pos);
    }
    else return Q_NULLPTR;
  }
  
  SvSignal* nextSignal()
  {
    return _pos < _signals->count() - 1 ? _signals->value(++_pos) : Q_NULLPTR;
  }
    
  
signals:
  void error(QString e);
  void connected();
  
};

#endif // SV_STORAGE_H

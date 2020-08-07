#include "sv_storage.h"


SvStorage::SvStorage(StorageParams params, sv::SvAbstractLogger &log, QObject *parent):
  QObject(parent),
  _log(log)
{
  _params = params;
  
  _signals.clear();

  _reconnect_timer = new QTimer();
  _reconnect_timer->setInterval(1000);

}

SvStorage::~SvStorage()
{
//  stop();
//  if(_thr)
//    delete _thr;

//  _thr->stop();

//  while(_thr)
//    QCoreApplication::processEvents();

//  /** при завершении работы сервера, сбрасываем все сигналы **/
//  foreach (SvSignal* signal, _signals)
//    signal->setLostValue();

  deleteLater();

}

bool SvStorage::init()
{
  try {

    _thr = new SvStorageThread(&_params, &_signals, this);

    if(!_thr->init())
      _exception.raise(_thr->lastError());

    return true;

  }

  catch(SvException& e) {

    _last_error = e.error;

    delete _thr;
    _thr = nullptr;

    // запускаем реконнект
    if(!_reconnect_timer->isActive()) // обязательно пррверяем!
      start_reconnect_timer();

    logerr(_last_error);

    return false;

  }
}

void SvStorage::logerr(QString e)
{
  _log << sv::log::TimeZZZ << sv::log::llError << e << '\n' << sv::log::endl;
}

void SvStorage::logreconnect()
{
  _log << sv::log::TimeZZZ << sv::log::llInfo << QString("Фух! Восстановлена связь с хранилищем %1 [%2:%3:%4]\n")
          .arg(_params.name).arg(_params.db_name)
          .arg(_params.host).arg(_params.port)
       << sv::log::endl;
}

void SvStorage::reconnect()
{
  if(init()) {

    stop_reconnect_timer();
    start();

    logreconnect();

  }
}

void SvStorage::start_reconnect_timer()
{
  _reconnect_timer->start();
  connect(_reconnect_timer, &QTimer::timeout, this, &SvStorage::reconnect);
}

void SvStorage::stop_reconnect_timer()
{
  _reconnect_timer->stop();
  disconnect(_reconnect_timer, &QTimer::timeout, this, &SvStorage::reconnect);
}

void SvStorage::stop()
{
  if(!_thr)
    return;

  /// при ручном завершении, отключаем переподключение
  disconnect(_thr, &SvStorageThread::finished, this, &SvStorage::start_reconnect_timer);
  disconnect(_thr, &SvStorageThread::connected, this, &SvStorage::logreconnect);

  stop_reconnect_timer();
  delete _reconnect_timer;

  _thr->stop();

}

void SvStorage::start()
{
  if(!_thr)
    return;

//  connect(_thr, &SvStorageThread::finished, _thr, &SvStorageThread::deleteLater);
  connect(_thr, &SvStorageThread::finished, this, &SvStorage::deleteThread);
//  connect(_thr, &SvStorageThread::finished, [=](){ delete _thr; _thr = nullptr; } );
  connect(_thr, &SvStorageThread::finished, this, &SvStorage::start_reconnect_timer);
  connect(_thr, &SvStorageThread::error, this, &SvStorage::logerr);

  _thr->start();
  
}

//void SvStorage::stopThread()
//{
//  _thr->stop();
//}

void SvStorage::deleteThread()
{
  delete _thr;
  _thr = nullptr;
}

/** *************  Storage Thread  ******************* */

SvStorageThread::SvStorageThread(StorageParams *params, QList<SvSignal*>* signalList, QObject *parent):
  QThread(parent),
  _finished(true)
{
  _params = params;
  
  _signals = signalList;
  
}

bool SvStorageThread::init()
{
  try {

    PGDB = new SvPGDB();
    PGDB->setConnectionParams(_params->db_name, _params->host, _params->port,
                              _params->login, _params->pass);

    QSqlError err = PGDB->connectToDB(QString("PGConn_%1").arg(_params->id));
    if(err.type() != QSqlError::NoError)
      _exception.raise(err.text());

    return true;
    
  }
  
  catch(SvException& e) {

    _last_error = e.error;
    emit error(e.error);
    return false;
    
  }
}

SvStorageThread::~SvStorageThread()
{
//  stop();
  delete PGDB;
  QSqlDatabase::removeDatabase(QString("PGConn_%1").arg(_params->id));

  deleteLater();
}

void SvStorageThread::stop()
{
  _started = false; 
  while(!_finished) QCoreApplication::instance()->processEvents();
}

void SvStorageThread::run()
{
  QTime elapsed_time = QTime::currentTime();
    
  elapsed_time.start();
  
  _started = true;
  _finished = false;

  bool _need_to_finish = false;

  while(!_need_to_finish) {
    
    msleep(1); // чтоб не грузило систему
    
    if(elapsed_time.elapsed() < 1000 )
      continue;

    
    elapsed_time.restart();
    
    QString signals_values = "";
    QString signals_reserve_values = "";

    SvSignal* signal = firstSignal();

    while(signal) {

      if(!_started)
        break;

#ifndef TEST_VALUES
        qsrand(QDateTime::currentMSecsSinceEpoch());
        if(signal->info()->data_type == 0)
            signal->setValue(qreal((qrand()%2)));
        else
            signal->setValue(qreal((qrand() & 0xFF) / 255.0));
//        qDebug() << signal->value();
#endif

      if((signal->info()->timeout >  0 && signal->isAlive()) ||
         (signal->info()->timeout == 0 && signal->isDeviceAlive())) {

        signals_values += QString("%1;%2|").arg(signal->id()).arg(signal->value());
      
      }

      /* алгоритм при потере сигнала:
       * 1) если для сигнала назначен резервный сигнал, то сигналу присваивается значение резервного сигнала
       * 2) если резервного сигнала нет, то присваиваем сигналу значение timeout_value,
       * 3) в первый раз зписываем в базу через функцию set_values. таким образом фиксируем в базе время потери связи
       * 4) впоследующем, если сигнал уже имеет значение timeout_value, то пропускаем его фиксацию в базе, для экономии ресурсов
       */
      else {
        
        if(signal->info()->timeout_signal_id > 0)
          signals_reserve_values += QString("%1;%2|").arg(signal->id()).arg(signal->info()->timeout_signal_id);


        else

          if(signal->setLostValue())
            signals_values += QString("%1;%2|").arg(signal->id()).arg(signal->info()->timeout_value);
        
      }

      
      signal = nextSignal(); 
        
    }

    /** здесь проверяем флаг _started. если _started = false, то есть пришел внешний сигнал на завершение потока,
     * то проходим по всем сигналам, и присваиваем им timeout_value.
     * присваиваем _need_to_finish = true. после прохода по всем сигналам, будет произведена запись в БД
     * на следующем прходе, главный цикл завершится, т.к. _need_to_finish уже true
     * такая схема применена для гарантированной записи в  значений timeout_value при завершении работы сервера */
    if(!_started)
    {
      signals_values = "";
      SvSignal* signal = firstSignal();

      while(signal) {

        signals_values += QString("%1;%2|").arg(signal->id()).arg(signal->info()->timeout_value);

        signal = nextSignal();

      }

      _need_to_finish = true;
    }


    try {

      if(!signals_values.isEmpty()) {

        signals_values.chop(1);
        QSqlError serr = PGDB->execSQL(QString(PG_FUNC_SET_VALUES).arg(signals_values));

        if(serr.type() != QSqlError::NoError)
          _exception.raise(serr.text());

      }

      if(!signals_reserve_values.isEmpty()) {

        signals_reserve_values.chop(1);
        QSqlError serr = PGDB->execSQL(QString(PG_FUNC_SET_RESERVE_VALUES).arg(signals_reserve_values));

        if(serr.type() != QSqlError::NoError)
          _exception.raise(serr.text());

      }
    }

    catch(SvException& e) {

      emit error(e.error);

      /// если произошла потеря связи с серверм БД, то завершаем поток
      _started = PGDB->connected();

      /// если нет, то продолжаем работать

    }
  }
  
  _finished = true;
  
}

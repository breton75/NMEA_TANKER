#ifndef PGDB_STORED_PROC_STORAGE_H
#define PGDB_STORED_PROC_STORAGE_H

#include "../../global/sv_abstract_storage.h"

#include "pgdb_stored_proc_global.h"

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
};

class PGDB_STORED_PROCSHARED_EXPORT PgdbStoredProcStorage: public SvAbstractStorage
{
  Q_OBJECT

public:
  PgdbStoredProcStorage();

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

#endif // PGDB_STORED_PROC_STORAGE_H

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
#include <QTimer>
#include <QtNetwork/QHostAddress>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../svlib/sv_pgdb.h"
#include "../../svlib/sv_abstract_logger.h"
#include "../../svlib/sv_exception.h"

#include "../global/sql_defs.h"
#include "../global/sv_signal.h"
#include "../global/defs.h"

#define DB_IMPERMISSIBLE_VALUE "Недопустимое значение параметра %1: %2.\n%3"

struct StorageParams
{
  int     id = -1;
  QString name = "";
  QString host = "";
  quint32 port = 5432;
  QString login = "";
  QString pass = "";
  QString db_name = "";

  static StorageParams fromJsonString(const QString& json_string) throw (SvException)
  {
    QJsonParseError err;
    QJsonDocument jd = QJsonDocument::fromJson(json_string.toUtf8(), &err);

    if(err.error != QJsonParseError::NoError)
      throw SvException(err.errorString());

    try {
      return fromJsonObject(jd.object());
    }
    catch(SvException e) {
      throw e;
    }
  }

  static StorageParams fromJsonObject(const QJsonObject &object) throw (SvException)
  {
    // проверяем наличие основных полей
    QStringList l = QStringList() << P_ID << P_NAME << P_HOST << P_PORT << P_DB;
    for(QString v: l)
      if(object.value(v).isUndefined())
        throw SvException(QString("В разделе \"storages\" отсутствует или не задан параметр '%1'").arg(v));

    StorageParams p;

    /* id */
    if(object.contains(P_ID))
    {
      p.id = object.value(P_ID).toInt(-1);
      if(p.id == -1)
        throw SvException(QString(DB_IMPERMISSIBLE_VALUE)
                               .arg(P_ID)
                               .arg(object.value(P_ID).toVariant().toString())
                               .arg("У каждого хранилища должен быть свой уникальный номер"));
    }

    /* name */
    if(object.contains(P_NAME)) {

      p.name = object.value(P_NAME).toString("");

      if(p.name.isEmpty())
        throw SvException(QString(DB_IMPERMISSIBLE_VALUE)
                          .arg(P_NAME)
                          .arg(object.value(P_NAME).toVariant().toString())
                          .arg("Имя хранилища не может быть пустым и должно быть заключено в двойные кавычки"));
    }

    /* host */
    if(object.contains(P_HOST)) {

      p.host = object.value(P_HOST).toString("");

      if(p.host.isEmpty() || (p.host.toLower() != "localhost" && QHostAddress(p.host).toIPv4Address() == 0))
        throw SvException(QString(DB_IMPERMISSIBLE_VALUE)
                          .arg(P_HOST)
                          .arg(object.value(P_HOST).toVariant().toString())
                          .arg("Допустимы \"localhost\" или ip адрес в формате \"192.16.2.100\""));
    }

    /* port */
    if(object.contains(P_PORT))
    {
      if(object.value(P_PORT).toInt(-1) < 1)
        throw SvException(QString(DB_IMPERMISSIBLE_VALUE)
                          .arg(P_PORT)
                          .arg(object.value(P_PORT).toVariant().toString())
                          .arg("Порт должен быть числом в диапазоне 1000 - 65535"));

      p.port = object.value(P_PORT).toInt(5432);

    }
    else p.port = 5432;

    /* user */
    if(object.contains(P_USER))
      p.login = object.value(P_USER).toString("postgres");

    else p.login = "postgres";

    /* pass */
    if(object.contains(P_PASS))
      p.pass = object.value(P_PASS).toString("postgres");

    else p.pass = "postgres";

    /* db */
    if(object.contains(P_DB)) {

      p.db_name = object.value(P_DB).toString("");

      if(p.db_name.isEmpty())
        throw SvException(QString(DB_IMPERMISSIBLE_VALUE)
                          .arg(P_DB)
                          .arg(object.value(P_DB).toVariant().toString())
                          .arg("Имя базы данных не может быть пустым и должно быть заключено в кавычки"));
    }


    return p;

  }

  QString toJsonString(QJsonDocument::JsonFormat format = QJsonDocument::Indented) const
  {
    QJsonDocument jd;
    jd.setObject(toJsonObject());

    return QString(jd.toJson(format));
  }

  QJsonObject toJsonObject() const
  {
    QJsonObject j;

    j.insert(P_ID, QJsonValue(static_cast<int>(id)).toInt());
    j.insert(P_NAME, QJsonValue(name).toString());
    j.insert(P_HOST, QJsonValue(host).toString());
    j.insert(P_PORT, QJsonValue(static_cast<int>(port)).toInt());
    j.insert(P_USER, QJsonValue(login).toString());
    j.insert(P_PASS, QJsonValue(pass).toString());
    j.insert(P_DB, QJsonValue(db_name).toString());

    return j;

  }
};


class SvStorageThread;

class SvStorage: public QObject
{
    Q_OBJECT
  
public:
  SvStorage(StorageParams params, sv::SvAbstractLogger &log, QObject *parent = Q_NULLPTR);
  ~SvStorage();
  
  int id() { return _params.id; }
  
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

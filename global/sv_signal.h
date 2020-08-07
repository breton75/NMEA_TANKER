#ifndef SV_SIGNAL_H
#define SV_SIGNAL_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include "../global/defs.h"

#include "../../svlib/sv_exception.h"

#define SIGINFO_IMPERMISSIBLE_VALUE "Недопустимое значение параметра %1: %2.\n%3"
#define SIGINFO_NO_PARAM  "В разделе \"devices\" отсутствует или не задан обязательный параметр \"%1\""

enum SignalDataTypes {
  dtInt = 0,
  dtFloat
};

struct SignalInfo
{
  SignalInfo() { }
  
  int         id = -1;
  QString     name = "";
  int         device_id = -1;
  QList<int>  storages;
  QString     type = "";
  bool        active = false;
  QString     description = "";
  int         timeout = 3000;
  int         timeout_value = -3;
  int         timeout_signal_id = -1;
  QString     params = "";
  QString     comment = "";

  static SignalInfo fromJsonString(const QString& json_string) throw (SvException)
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

  static SignalInfo fromJsonObject(const QJsonObject &object) throw (SvException)
  {
    // проверяем наличие основных полей
    QStringList l = QStringList() << P_ID << P_NAME << P_TYPE << P_DEVICE
                                  << P_TIMEOUT << P_TIMEOUT_VALUE;
    for(QString v: l)
      if(object.value(v).isUndefined())
        throw SvException(QString(SIGINFO_NO_PARAM).arg(v));

    QString P;
    SignalInfo p;

    /* id */
    P = P_ID;
    if(object.contains(P))
    {
      if(object.value(P).toInt(-1) == -1)
        throw SvException(QString(SIGINFO_IMPERMISSIBLE_VALUE)
                               .arg(P)
                               .arg(object.value(P).toVariant().toString())
                               .arg("У каждого сигнала должен быть свой уникальный номер"));

      p.id = object.value(P).toInt(-1);

    }
    else throw SvException(QString(SIGINFO_NO_PARAM).arg(P));


    /* name */
    P = P_NAME;
    if(object.contains(P)) {

      if(object.value(P).toString("").isEmpty())
        throw SvException(QString(SIGINFO_IMPERMISSIBLE_VALUE)
                          .arg(P)
                          .arg(object.value(P).toVariant().toString())
                          .arg("Имя сигнала не может быть пустым и должно быть заключено в двойные кавычки"));

      p.name = object.value(P).toString("");

    }
    else throw SvException(QString(SIGINFO_NO_PARAM).arg(P));

    /* device */
    P = P_DEVICE;
    if(object.contains(P))
    {
      if(object.value(P).toInt(-1) < 1)
        throw SvException(QString(SIGINFO_IMPERMISSIBLE_VALUE)
                               .arg(P)
                               .arg(object.value(P).toVariant().toString())
                               .arg(QString("%1. Неверно указан ID устройства, к которому относится сигнал.").arg(p.name)));

      p.device_id = object.value(P).toInt(-1);

    }
    else throw SvException(QString(SIGINFO_NO_PARAM).arg(P));

    /* storages */
    P = P_STORAGES;
    if(object.contains(P))
    {
      QJsonArray a = object.value(P).toArray();

      for(QJsonValue v: a)
      {
        int s = v.toInt(-1);
        if(s < 0)
          throw SvException(QString(SIGINFO_IMPERMISSIBLE_VALUE)
                                 .arg(P)
                                 .arg(object.value(P).toVariant().toString())
                                 .arg(QString("%1. Неверно указан ID хранилища, к которому относится сигнал.").arg(p.name)));

        if(!p.storages.contains(s))
          p.storages.append(s);
      }
    }
    else p.storages = QList<int>();

    /* active */
    P = P_ACTIVE;
    if(object.contains(P)) {

      p.active = object.value(P).toBool(true);

    }
    else p.active = true;

    /* type */
    P = P_TYPE;
    if(object.contains(P)) {

      if(object.value(P).toString("").isEmpty())
        throw SvException(QString(SIGINFO_IMPERMISSIBLE_VALUE)
                          .arg(P)
                          .arg(object.value(P).toVariant().toString())
                          .arg(QString("%1. Тип сигнала не может быть пустым и должен быть заключен в двойные кавычки").arg(p.name)));

      p.type = object.value(P).toString("");

    }
    else throw SvException(QString(SIGINFO_NO_PARAM).arg(P));


    /* params */
    P = P_PARAMS;
    if(object.contains(P)) {

      p.params = QString(QJsonDocument(object.value(P).toObject()).toJson(QJsonDocument::Compact));

    }
    else p.params = "\"{ }\"";

    /* timeout*/
    P = P_TIMEOUT;
    if(object.contains(P))
    {
      if(object.value(P).toInt(-1) < 1)
        throw SvException(QString(SIGINFO_IMPERMISSIBLE_VALUE)
                               .arg(P)
                               .arg(object.value(P).toVariant().toString())
                               .arg(QString("%1. Таймаут не может быть меньше 1 мсек.").arg(p.name)));

      p.timeout = object.value(P).toInt(3000);

    }
    else p.timeout = 3000;

    /* timeout_value */
    P = P_TIMEOUT_VALUE;
    if(object.contains(P))
    {
      p.timeout_value = object.value(P).toInt(-1);

    }
    else p.timeout_value = -1;

    /* timeout_signal_id */
    P = P_TIMEOUT_SIGNAL_ID;
    if(object.contains(P))
    {
      p.timeout_signal_id = object.value(P).toInt(-1);

    }
    else p.timeout_signal_id = -1;

    /* description */
    P = P_DESCRIPTION;
    if(object.contains(P)) {

      p.description = object.value(P).toString("");

    }
    else p.description = "";

    /* comment */
    P = P_COMMENT;
    if(object.contains(P)) {

      p.comment = object.value(P).toString("");

    }
    else p.comment = "";


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
    QJsonArray a;

    for(int v: storages)
      a << QJsonValue(v);

    j.insert(P_ID, QJsonValue(static_cast<int>(id)).toInt());
    j.insert(P_NAME, QJsonValue(name).toString());
    j.insert(P_ACTIVE, QJsonValue(active).toBool());
    j.insert(P_DEVICE, QJsonValue(device_id).toInt());
    j.insert(P_STORAGES, QJsonValue(a));
    j.insert(P_TYPE, QJsonValue(type).toString());
    j.insert(P_PARAMS, QJsonValue(params).toString());
    j.insert(P_TIMEOUT, QJsonValue(timeout).toInt());
    j.insert(P_TIMEOUT_VALUE, QJsonValue(timeout_value).toInt());
    j.insert(P_TIMEOUT_SIGNAL_ID, QJsonValue(timeout_signal_id).toInt());
    j.insert(P_DESCRIPTION, QJsonValue(description).toString());
    j.insert(P_COMMENT, QJsonValue(comment).toString());

    return j;

  }
};

class SvSignal: public QObject
{
  Q_OBJECT
  
public:
  explicit SvSignal(SignalInfo& info);
  ~SvSignal();
  
  int id() { return p_info.id; }
  
  void setup(const SignalInfo& info) { p_info = info; }
  const SignalInfo* info() { return &p_info; }
  
  quint64   lostEpoch()  const { return p_lost_epoch; }
  QDateTime lastUpdate() const { return p_last_update; }

  qreal value() const { return p_value; }
  bool isAlive() { return p_lost_epoch > quint64(QDateTime::currentMSecsSinceEpoch()); }

  void setDeviceLostEpoch(const quint64 lost_epoch) { p_device_lost_epoch = lost_epoch; }
  bool isDeviceAlive() { return p_device_lost_epoch > quint64(QDateTime::currentMSecsSinceEpoch()); }

  bool operator==(SvSignal& other) const
  { 
    return p_info.id == other.info()->id;
  }
  
  bool operator==(SvSignal* other) const
  { 
    return p_info.id == other->info()->id;
  }

  qreal previousValue() const { return p_previous_value; }

  bool setLostValue()
  {
    if(p_value != p_info.timeout_value) {

      p_previous_value = p_value;
      p_value = p_info.timeout_value;
      return true;
    }

    return false;

  }
  
private:
  SignalInfo p_info;
  
  QDateTime p_last_update;
  quint64 p_lost_epoch = 0;
  quint64 p_device_lost_epoch = 0;

  qreal p_value = 0.0;
  qreal p_previous_value = 0.0;
  
public slots:
  void setValue(qreal value);
  
  
};

#endif // SV_SIGNAL_H

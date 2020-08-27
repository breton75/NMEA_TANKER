#ifndef DEVICE_PARAMS
#define DEVICE_PARAMS

#include <QtCore>
#include <QJsonDocument>

#include "conning_kongsber_device_global.h"

#include "../../svlib/sv_exception.h"

#define RESET_INTERVAL  10

// имена параметров устройства
#define P_HEADER  "header"
//#define P_TALKER  "talker"
//#define P_NAME    "name"
#define P_RESET_TIMEOUT   "reset_timeout"

#define DEV_IMPERMISSIBLE_VALUE "Недопустимое значение параметра \"%1\": %2.\n%3"

//namespace dev {


  struct DeviceParams {

//    QString   header = "[$]II(XDR|GEN),";
//    QString   name   = "XDR";
    quint16   reset_timeout = RESET_INTERVAL;

    bool isValid = true;

    static DeviceParams fromJson(const QString& json_string) throw (SvException)
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

    static DeviceParams fromJsonObject(const QJsonObject &object) throw (SvException)
    {
      DeviceParams p;

//      QString P = P_HEADER;
//      if(object.contains(P)) {

//        if(object.value(P).toString("").isEmpty())
//          throw SvException(QString(DEV_IMPERMISSIBLE_VALUE)
//                            .arg(P).arg(object.value(P).toVariant().toString())
//                            .arg("Шаблон заголовка сообщения не может быть пустым"));

//        p.header = object.value(P).toString("");

//      }

//      if(object.contains(P_NAME)) {

//        if(object.value(P_NAME).toString("").isEmpty())
//          throw SvException(QString(DEV_IMPERMISSIBLE_VALUE)
//                            .arg(P_NAME).arg(object.value(P_NAME).toVariant().toString())
//                            .arg("Имя отправителя не может быть пустым"));

//        p.talker = object.value(P_TALKER).toString("");

//      }

      if(object.contains(P_RESET_TIMEOUT)) {

        if(object.value(P_RESET_TIMEOUT).toInt(-1) < 1)
          throw SvException(QString(DEV_IMPERMISSIBLE_VALUE)
                            .arg(P_RESET_TIMEOUT)
                            .arg(object.value(P_RESET_TIMEOUT).toVariant().toString())
                            .arg("Период сброса не может быть меньше 1 мсек."));

        p.reset_timeout = object.value(P_RESET_TIMEOUT).toInt(RESET_INTERVAL);

    //qDebug() << p.isValid << P_RESET_TIMEOUT << p.reset_timeout;
    //        p.isValid = p.isValid && (p.reset_timeout > 0);

      }

      return p;

    }

    QString toString(QJsonDocument::JsonFormat format = QJsonDocument::Indented) const
    {
      QJsonDocument jd;
      jd.setObject(toJsonObject());

      return QString(jd.toJson(format));
    }

    QJsonObject toJsonObject() const
    {
      QJsonObject j;

//      j.insert(P_HEADER, QJsonValue(header).toString());
//      j.insert(P_NAME, QJsonValue(name).toString());
      j.insert(P_RESET_TIMEOUT, QJsonValue(reset_timeout));

      return j;

    }

  };


#endif // DEVICE_PARAMS


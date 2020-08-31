#ifndef SIGNAL_PARAMS_H
#define SIGNAL_PARAMS_H

#include <QJsonDocument>
#include <QJsonObject>

#include "../../svlib/sv_exception.h"

#include "../global/sv_signal.h"

#define P_GROUP "group"
#define P_WORD  "word"
#define P_TAG   "tag"

#define S_IMPERMISSIBLE_VALUE "Недопустимое значение параметра %1: %2.\n%3"
#define S_NO_PARAM  "Отсутствует или не задан обязательный параметр \"%1\""

namespace  ckng {

  struct SignalParams {

    int group;
    int word;
    QString tag;

    static SignalParams fromSignal(SvSignal* signal) throw (SvException)
    {
      try
      {

        return SignalParams::fromJsonString(signal->config()->params);
      }
      catch(SvException& e)
      {
        throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name).arg(e.error));
      }
    }

    static SignalParams fromJsonString(const QString& json_string) throw (SvException)
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

    static SignalParams fromJsonObject(const QJsonObject &object) throw (SvException)
    {
      SignalParams p;

      /** tag **/
      if(object.contains(P_TAG)) {

        p.tag = object.value(P_TAG).toString("");

      }
      else p.tag = object.value(P_TAG).toString("");


      /** group **/
      if(object.contains(P_GROUP)) {

        if(object.value(P_GROUP).toInt(-1) < 0)
          throw SvException(QString(S_IMPERMISSIBLE_VALUE)
                            .arg(P_GROUP)
                            .arg(object.value(P_GROUP).toVariant().toString())
                            .arg("Допустимы числовые значения больше 0"));

        p.group = object.value(P_GROUP).toInt(-1);

      }
      else
        throw SvException(QString(S_NO_PARAM).arg(P_GROUP));


      /** word **/
      if(object.contains(P_WORD)) {

        if(object.value(P_WORD).toInt(-1) < 0)
          throw SvException(QString(S_IMPERMISSIBLE_VALUE)
                            .arg(P_WORD)
                            .arg(object.value(P_WORD).toVariant().toString())
                            .arg("Допустимы числовые значения больше 0"));

        p.word = object.value(P_WORD).toInt(-1);
      }
      else
        throw SvException(QString(S_NO_PARAM).arg(P_WORD));


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

      j.insert(P_TAG,   QJsonValue(static_cast<int>(group)).toInt());
      j.insert(P_WORD,   QJsonValue(static_cast<int>(word)).toInt());
      j.insert(P_GROUP, QJsonValue(tag).toString());

      return j;

    }
  };
}


#endif // SIGNAL_PARAMS_H

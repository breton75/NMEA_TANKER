/**********************************************************************
 *  константы и структуры для работы с последовательным портом.
 *  форма для редактирования находится в файлах sv_serial_editor.*
 *  ВАЖНО! решено разнести парметры и редактор по разным фалам,
 *  чтобы не было проблемы при работе с приложениями без GUI
 *
 *  автор Свиридов С.А. Авиационные и Морская Электроника
 * *********************************************************************/

#ifndef VIRTUAL_DEVICE_UDP_PARAMS
#define VIRTUAL_DEVICE_UDP_PARAMS

#include <QtGlobal>
#include <QtCore/QCommandLineParser>
#include <QUdpSocket>
#include <QHostAddress>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../svlib/sv_exception.h"

// имена параметров для UDP
#define P_UDP_HOST      "host"
#define P_UDP_RECV_PORT "recv_port"
#define P_UDP_SEND_PORT "send_port"

#define UDP_IMPERMISSIBLE_VALUE "Недопустимое значение параметра \"%1\": %2.\n%3"

const QMap<QString, QHostAddress::SpecialAddress> SpecialHosts = {{"localhost", QHostAddress::LocalHost},
                                                                  {"any", QHostAddress::Any},
                                                                  {"broadcast", QHostAddress::Broadcast}};

/** структура для хранения параметров udp **/
struct UdpParams {

  QHostAddress host = QHostAddress::Any;
  quint16 recv_port = 6001;
  quint16 send_port = 5001;

  static UdpParams fromJsonString(const QString& json_string) throw (SvException)
  {
    QJsonParseError err;
    QJsonDocument jd = QJsonDocument::fromJson(json_string.toUtf8(), &err);

//    SvException excpt;

    if(err.error != QJsonParseError::NoError)
      throw SvException(err.errorString());

    try {
      return fromJsonObject(jd.object());
    }
    catch(SvException e) {
      throw e;
    }
  }

  static UdpParams fromJsonObject(const QJsonObject &object) throw (SvException)
  {
    UdpParams p;
    SvException excpt;

    if(object.contains(P_UDP_HOST)) {

      QString host = object.value(P_UDP_HOST).toString("").toLower();

      if(SpecialHosts.contains(host)) p.host = QHostAddress(SpecialHosts.value(host));

      else
      if(QHostAddress(host).toIPv4Address() != 0) p.host = QHostAddress(host);

      else
        throw excpt.assign(QString(UDP_IMPERMISSIBLE_VALUE)
                           .arg(P_UDP_HOST).arg(object.value(P_UDP_HOST).toVariant().toString())
                           .arg("Допускаются ip адреса в формате 129.168.1.1, а также слова localhost, any, broadcast"));
    }

    if(object.contains(P_UDP_RECV_PORT)) {

      p.recv_port = object.value(P_UDP_RECV_PORT).toInt(0);

      if(p.recv_port == 0)
        throw excpt.assign(QString(UDP_IMPERMISSIBLE_VALUE)
                           .arg(P_UDP_RECV_PORT).arg(object.value(P_UDP_RECV_PORT).toVariant().toString())
                           .arg("Допускаются тоьлко числовые значения"));

    }

    if(object.contains(P_UDP_SEND_PORT))
    {

      p.send_port = object.value(P_UDP_SEND_PORT).toInt(0);

      if(p.send_port == 0)
        throw excpt.assign(QString(UDP_IMPERMISSIBLE_VALUE)
                           .arg(P_UDP_SEND_PORT).arg(object.value(P_UDP_SEND_PORT).toVariant().toString())
                           .arg("Допускаются тоьлко числовые значения"));

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

    j.insert(P_UDP_HOST, QJsonValue(host.toString()).toString());
    j.insert(P_UDP_RECV_PORT, QJsonValue(static_cast<int>(recv_port)).toInt());
    j.insert(P_UDP_SEND_PORT, QJsonValue(static_cast<int>(send_port)).toInt());

    return j;

  }
};


#endif // VIRTUAL_DEVICE_UDP_PARAMS


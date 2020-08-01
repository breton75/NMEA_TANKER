#ifndef DEVICE_PARAMS_H
#define DEVICE_PARAMS_H

#include <QMap>
#include <QString>
#include <QtCore>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../svlib/sv_exception.h"


#define RESET_INTERVAL  10

// имена параметров устройств
#define P_START_REGISTER  "start_register"
#define P_LAST_REGISTER  "last_register"
#define P_RESET_TIMEOUT   "reset_timeout"

namespace dev {

#ifdef VERSION_2


  struct DeviceParams {

    quint16   start_register = 0;
    quint16   last_register = 0;
    quint16   reset_timeout = RESET_INTERVAL;

    bool isValid = true;

    static DeviceParams fromJson(const QString& json_string)
    {
      QJsonDocument jd = QJsonDocument::fromJson(json_string.toUtf8());
      return fromJsonObject(jd.object());
    }

    static DeviceParams fromJsonObject(const QJsonObject &object)
    {
//      qDebug() << object.contains(P_START_REGISTER) << object.contains(P_RESET_TIMEOUT);
      DeviceParams p;

      if(object.contains(P_START_REGISTER)) {

        QByteArray h = object.value(P_START_REGISTER).toString().toUtf8();

        bool ok = false;
        p.start_register = h.toUShort(&ok, 0);
        p.isValid = p.isValid && ok;

      }

      if(object.contains(P_LAST_REGISTER)) {

        QByteArray h = object.value(P_LAST_REGISTER).toString().toUtf8();

        bool ok = false;
        p.last_register = h.toUShort(&ok, 0);
        p.isValid = p.isValid && ok;

        p.isValid = p.last_register >= p.start_register;

      }

      if(object.contains(P_RESET_TIMEOUT)) {

        p.reset_timeout = object.value(P_RESET_TIMEOUT).toInt(RESET_INTERVAL);
//qDebug() << p.isValid << P_RESET_TIMEOUT << p.reset_timeout;
        p.isValid = p.isValid && (p.reset_timeout > 0);

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
      QString r;

      r = QString::number(start_register, 16);

      if(r.length() < 4)
        r.push_front(QString(4 - r.length(), QChar('0')));

      QString start_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);


      r = QString::number(last_register, 16);

      if(r.length() < 4)
        r.push_front(QString(4 - r.length(), QChar('0')));

      QString last_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);


      j.insert(P_START_REGISTER, QJsonValue(start_r));
      j.insert(P_LAST_REGISTER, QJsonValue(last_r));
      j.insert(P_RESET_TIMEOUT, QJsonValue(reset_timeout));

      return j;

    }

  };

  /** для версии 1 **/
  #else


  #define P_SERIAL_BAUDRATE "baudrate"
  #define P_SERIAL_PORTNAME "portname"
  #define P_SERIAL_DATABITS "databits"
  #define P_SERIAL_PARITY   "parity"
  #define P_SERIAL_STOPBITS "stopbits"
  #define P_SERIAL_FLOWCTRL "flowcontrol"
  #define P_DEVICE_PROTOCOL "protocol"

    struct SerialPortParams {
      QString portname = "tty0";
      quint32 baudrate = 9600;
      QSerialPort::DataBits databits = QSerialPort::Data8;
      QSerialPort::Parity parity = QSerialPort::NoParity;
      QSerialPort::StopBits stopbits = QSerialPort::OneStop;
      QSerialPort::FlowControl flowcontrol = QSerialPort::NoFlowControl;
      QString description = "";
    };

  class SerialParamsParser
  {
  public:
    SerialParamsParser(QString params_string) {

      _params_string = params_string;
    }

    QString lastError() { return _last_error; }

    SerialPortParams serialParams() { return _serial_params; }

    static QString getSring(SerialPortParams params)
    {

      QString result = QString("-%1=%2 -%3=%4 -%5=%6 -%7=%8 -%9=%10 -%11=%12")
                  .arg(P_SERIAL_PORTNAME).arg(params.portname)
                  .arg(P_SERIAL_BAUDRATE).arg(params.baudrate)
                  .arg(P_SERIAL_DATABITS).arg(params.databits)
                  .arg(P_SERIAL_PARITY  ).arg(params.parity)
                  .arg(P_SERIAL_STOPBITS).arg(params.stopbits)
                  .arg(P_SERIAL_FLOWCTRL).arg(params.flowcontrol);

      return result;

    }

    bool parse()
    {
      //! обязателен первый аргумент!! парсер считает, что там находится путь к программе
      QStringList params_list;
      params_list << "dumb_path_to_app" << _params_string.split(" ");

      QCommandLineParser parser;
      parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

      parser.addOption(QCommandLineOption(P_SERIAL_BAUDRATE, "BAUDRATE", "19200", "19200"));
      parser.addOption(QCommandLineOption(P_SERIAL_PORTNAME, "PORTNAME", "0", "0"));
      parser.addOption(QCommandLineOption(P_SERIAL_DATABITS, "DATABITS", "8", "8"));
      parser.addOption(QCommandLineOption(P_SERIAL_PARITY,   "PARITY",   "0", "0"));
      parser.addOption(QCommandLineOption(P_SERIAL_STOPBITS, "STOPBITS", "2", "2"));
      parser.addOption(QCommandLineOption(P_SERIAL_FLOWCTRL, "FLOWCTRL", "0", "0"));
      parser.addOption(QCommandLineOption(P_DEVICE_PROTOCOL, "PROTOCOL", "1", "1"));


      quint32 d;
      bool ok;

      try {

        if(!parser.parse(params_list)) _exception.raise(QString("Неверное значение параметра: %1").arg(_params_string));

        _serial_params.baudrate = parser.value(P_SERIAL_BAUDRATE).toUInt(&ok);
        if(!ok) _exception.raise(QString("Неверное значение параметра: %1").arg(P_SERIAL_BAUDRATE));

        d = parser.value(P_SERIAL_DATABITS).toUInt(&ok);
        if(!(ok & DataBits.contains(QSerialPort::DataBits(d)))) _exception.raise(QString("Неверное значение параметра: %1").arg(P_SERIAL_DATABITS));
        _serial_params.databits = QSerialPort::DataBits(d);

        d = parser.value(P_SERIAL_FLOWCTRL).toUInt(&ok);
        if(!(ok & FlowControls.contains(QSerialPort::FlowControl(d)))) _exception.raise(QString("Неверное значение параметра: %1").arg(P_SERIAL_FLOWCTRL));
        _serial_params.flowcontrol = QSerialPort::FlowControl(d);

        _serial_params.portname = parser.value(P_SERIAL_PORTNAME); //.toUpper();

        d = parser.value(P_SERIAL_PARITY).toUInt(&ok);
        if(!ok) _exception.raise(QString("Неверное значение параметра: %1").arg(P_SERIAL_PARITY));
        _serial_params.parity = QSerialPort::Parity(d);

        d = parser.value(P_SERIAL_STOPBITS).toUInt(&ok);
        if(!ok) _exception.raise(QString("Неверное значение параметра: %1").arg(P_SERIAL_STOPBITS));
        _serial_params.stopbits = QSerialPort::StopBits(d);

        return true;

      }

      catch(SvException& e) {
        _last_error = e.error;
        return false;
      }
    }

  private:
    QString _params_string = "";
    SerialPortParams _serial_params;

    QString _last_error = "";

    SvException _exception;

  };

#endif

}

#endif // DEVICE_PARAMS_H

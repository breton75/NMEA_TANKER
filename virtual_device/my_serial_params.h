/**********************************************************************
 *  константы и структуры для работы с последовательным портом.
 *  форма для редактирования находится в файлах sv_serial_editor.*
 *  ВАЖНО! решено разнести парметры и редактор по разным фалам,
 *  чтобы не было проблемы при работе с приложениями без GUI
 *
 *  автор Свиридов С.А. Авиационные и Морская Электроника
 * *********************************************************************/

#ifndef VIRTUAL_DEVICE_SERIAL_PARAMS
#define VIRTUAL_DEVICE_SERIAL_PARAMS

#include <QtGlobal>
#include <QtCore/QCommandLineParser>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../svlib/sv_exception.h"

// имена параметров для RS
#define P_SERIAL_BAUDRATE "baudrate"
#define P_SERIAL_PORTNAME "portname"
#define P_SERIAL_DATABITS "databits"
#define P_SERIAL_PARITY   "parity"
#define P_SERIAL_STOPBITS "stopbits"
#define P_SERIAL_FLOWCTRL "flowcontrol"


const QList<int> Baudrates = {75, 115, 134, 150, 300, 600, 1200, 1800, 2400, 4800, 7200, 9600, 14400, 19200, 38400, 57600, 115200, 128000};

const QMap<QSerialPort::DataBits, QString>    DataBits     = {{QSerialPort::Data5, "5"},
                                                              {QSerialPort::Data6, "6"},
                                                              {QSerialPort::Data7, "7"},
                                                              {QSerialPort::Data8, "8"}};

const QMap<QSerialPort::Parity, QString>      Parities     = {{QSerialPort::NoParity, "Нет"},
                                                              {QSerialPort::EvenParity, "Чет"},
                                                              {QSerialPort::OddParity, "Нечет"},
                                                              {QSerialPort::SpaceParity, "Пробел"},
                                                              {QSerialPort::MarkParity, "Маркер"}};

const QMap<QSerialPort::StopBits, QString>    StopBits     = {{QSerialPort::OneStop, "1"},
                                                              {QSerialPort::OneAndHalfStop, "1,5"},
                                                              {QSerialPort::TwoStop, "2"}};

const QMap<QSerialPort::FlowControl, QString> FlowControls = {{QSerialPort::NoFlowControl, "Нет"},
                                                              {QSerialPort::HardwareControl, "Аппаратное"},
                                                              {QSerialPort::SoftwareControl, "Программное"}};


/** структура для хранения параметров последовательного порта **/
struct SerialParams {

  QString                   portname    =     "ttyS0";
  quint32                   baudrate    =     19200;
  QSerialPort::DataBits     databits    =     QSerialPort::Data8;
  QSerialPort::Parity       parity      =     QSerialPort::NoParity;
  QSerialPort::StopBits     stopbits    =     QSerialPort::OneStop;
  QSerialPort::FlowControl  flowcontrol =     QSerialPort::NoFlowControl;

  bool isValid = true;

  static SerialParams fromJsonString(const QString& json_string)
  {
    QJsonDocument jd = QJsonDocument::fromJson(json_string.toUtf8());
    return fromJsonObject(jd.object());
  }

  static SerialParams fromJsonObject(const QJsonObject &object)
  {
    SerialParams p;

    if(object.contains(P_SERIAL_PORTNAME))
      p.portname = object.value(P_SERIAL_PORTNAME).toString("ttyS0");

    if(object.contains(P_SERIAL_BAUDRATE))
      p.baudrate = object.value(P_SERIAL_BAUDRATE).toInt(19200);

    if(object.contains(P_SERIAL_DATABITS))
      p.databits = QSerialPort::DataBits(object.value(P_SERIAL_DATABITS).toInt(QSerialPort::Data8));

    if(object.contains(P_SERIAL_PARITY))
      p.parity = QSerialPort::Parity(object.value(P_SERIAL_PARITY).toInt(QSerialPort::NoParity));

    if(object.contains(P_SERIAL_STOPBITS))
      p.stopbits = QSerialPort::StopBits(object.value(P_SERIAL_STOPBITS).toInt(QSerialPort::OneStop));

    if(object.contains(P_SERIAL_FLOWCTRL))
      p.flowcontrol = QSerialPort::FlowControl(object.value(P_SERIAL_FLOWCTRL).toInt(QSerialPort::NoFlowControl));

    return p;

  }

  QString toJsonString() const
  {
    QJsonDocument jd;
    jd.setObject(toJsonObject());

    return QString(jd.toJson(QJsonDocument::Indented));
  }

  QJsonObject toJsonObject() const
  {
    QJsonObject j;

    j.insert(P_SERIAL_PORTNAME, QJsonValue(portname).toString());
    j.insert(P_SERIAL_BAUDRATE, QJsonValue(static_cast<int>(baudrate)).toInt());
    j.insert(P_SERIAL_DATABITS, QJsonValue(static_cast<int>(databits)).toInt());
    j.insert(P_SERIAL_FLOWCTRL, QJsonValue(static_cast<int>(flowcontrol)).toInt());
    j.insert(P_SERIAL_PARITY,   QJsonValue(static_cast<int>(parity)).toInt());
    j.insert(P_SERIAL_STOPBITS, QJsonValue(static_cast<int>(stopbits)).toInt());

    return j;

  }
};


#endif // VIRTUAL_DEVICE_SERIAL_PARAMS


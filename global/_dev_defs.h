#ifndef SV_DEV_DEFS_H
#define SV_DEV_DEFS_H

#include <QtWidgets/QDialog>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMap>
#include <QtCore/QCommandLineParser>

#include "../../svlib/sv_exception.h"

//namespace devd {

//    quint16 MODBUS_CRC16 (const quint8 *nData, quint16 wLength);

//}


static const quint16 wCRCTable[] = {
0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

class CRC
{
public:
    CRC() {}

    static quint16 MODBUS_CRC16 (const quint8 *nData, quint16 wLength)
    {

      quint8 nTemp;
      quint16 wCRCWord = 0xFFFF;

       while (wLength--)
       {
            nTemp = *nData++ ^ wCRCWord;
            wCRCWord >>= 8;
            wCRCWord ^= wCRCTable[nTemp];
       }

       return wCRCWord;

    }

    static inline uint16_t crc16_ccitt_byte(uint8_t byte, uint16_t crc)
    {

        /* For each bit in the data byte, starting from the leftmost bit */
        for (int i = 7; i >= 0; i--) {

            /* If leftmost bit of the CRC is 1, we will XOR with the polynomial later */
            int xor_flag = crc & 0x8000;

            /* Shift the CRC, and append the next bit of the message to the rightmost side of the CRC */
            crc <<= 1;
            crc |= (byte & (1 << i)) ? 1 : 0;

            /* Perform the XOR with the polynomial */
            if (xor_flag)
                crc ^= 0x1021;

        }

        return crc;
    }

    static uint16_t CRC16_CCITT (const quint8 *nData, uint16_t wLength)
    {
        uint16_t crc = 0xFFFF;

        /* Update the CRC using the data */
        for (int i = 0; i < wLength; i++)
            crc = crc16_ccitt_byte(nData[i], crc);

        for (int i = 0; i < 2; i++)
            crc = crc16_ccitt_byte(0, crc);

        return crc;
    }

};

  
  const QList<int> Baudrates = {75, 115, 134, 150, 300, 600, 1200, 1800, 2400, 4800, 7200, 9600, 14400, 19200, 38400, 57600, 115200, 128000};
  
  const QMap<QSerialPort::DataBits, QString> DataBits = {{QSerialPort::Data5, "5"}, 
                                                         {QSerialPort::Data6, "6"},
                                                         {QSerialPort::Data7, "7"},
                                                         {QSerialPort::Data8, "8"}};
  
  const QMap<QSerialPort::Parity, QString> Parities = {{QSerialPort::NoParity, "Нет"},
                                                       {QSerialPort::EvenParity, "Чет"},
                                                       {QSerialPort::OddParity, "Нечет"},
                                                       {QSerialPort::SpaceParity, "Пробел"},
                                                       {QSerialPort::MarkParity, "Маркер"}};
  
  const QMap<QSerialPort::StopBits, QString> StopBits = {{QSerialPort::OneStop, "1"},
                                                         {QSerialPort::OneAndHalfStop, "1,5"},
                                                         {QSerialPort::TwoStop, "2"}};
  
  const QMap<QSerialPort::FlowControl, QString> FlowControls = {{QSerialPort::NoFlowControl, "Нет"},
                                                                {QSerialPort::HardwareControl, "Аппаратное"},
                                                                {QSerialPort::SoftwareControl, "Программное"}};



#ifdef VERSION_2

  // имена параметров устройств
#define P_SERIAL_BAUDRATE "baudrate"
#define P_SERIAL_PORTNAME "portname"
#define P_SERIAL_DATABITS "databits"
#define P_SERIAL_PARITY   "parity"
#define P_SERIAL_STOPBITS "stopbits"
#define P_SERIAL_FLOWCTRL "flowcontrol"
#define P_ADDRESS         "address"

struct DeviceParams {

  QString                   portname =        "tty0";
  quint32                   baudrate =        19200;
  QSerialPort::DataBits     databits =        QSerialPort::Data8;
  QSerialPort::Parity       parity =          QSerialPort::NoParity;
  QSerialPort::StopBits     stopbits =        QSerialPort::OneStop;
  QSerialPort::FlowControl  flowcontrol =     QSerialPort::NoFlowControl;
  quint32                   address =         0;

};

class DeviceParamsParser
{
public:
  DeviceParamsParser(const QString& params_string):
    _params_string(params_string)
  {   }

  QString lastError() { return _last_error; }

  DeviceParams params() { return _params; }

  static QString getSring(DeviceParams params)
  {

    QString result = QString("-%1=%2 -%3=%4 -%5=%6 -%7=%8 -%9=%10 -%11=%12 -%13=14")
                .arg(P_SERIAL_PORTNAME).arg(params.portname)
                .arg(P_SERIAL_BAUDRATE).arg(params.baudrate)
                .arg(P_SERIAL_DATABITS).arg(params.databits)
                .arg(P_SERIAL_PARITY  ).arg(params.parity)
                .arg(P_SERIAL_STOPBITS).arg(params.stopbits)
                .arg(P_SERIAL_FLOWCTRL).arg(params.flowcontrol)
                .arg(P_ADDRESS).arg(params.address);

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
    parser.addOption(QCommandLineOption(P_ADDRESS,         "ADDRESS",  "0", "0"));


    quint32 d;
    bool ok;

    try {

      if(!parser.parse(params_list)) _exception.raise(QString("Неверное значение параметра: %1").arg(_params_string));

      _params.baudrate = parser.value(P_SERIAL_BAUDRATE).toUInt(&ok);
      if(!ok) _exception.raise(QString("Неверное значение параметра: %1").arg(P_SERIAL_BAUDRATE));

      d = parser.value(P_SERIAL_DATABITS).toUInt(&ok);
      if(!(ok & DataBits.contains(QSerialPort::DataBits(d)))) _exception.raise(QString("Неверное значение параметра: %1").arg(P_SERIAL_DATABITS));
      _params.databits = QSerialPort::DataBits(d);

      d = parser.value(P_SERIAL_FLOWCTRL).toUInt(&ok);
      if(!(ok & FlowControls.contains(QSerialPort::FlowControl(d)))) _exception.raise(QString("Неверное значение параметра: %1").arg(P_SERIAL_FLOWCTRL));
      _params.flowcontrol = QSerialPort::FlowControl(d);

      _params.portname = parser.value(P_SERIAL_PORTNAME); //.toUpper();

      d = parser.value(P_SERIAL_PARITY).toUInt(&ok);
      if(!ok) _exception.raise(QString("Неверное значение параметра: %1").arg(P_SERIAL_PARITY));
      _params.parity = QSerialPort::Parity(d);

      d = parser.value(P_SERIAL_STOPBITS).toUInt(&ok);
      if(!ok) _exception.raise(QString("Неверное значение параметра: %1").arg(P_SERIAL_STOPBITS));
      _params.stopbits = QSerialPort::StopBits(d);

      _params.address = parser.value(P_ADDRESS).toUInt(&ok);
      if(!ok) _exception.raise(QString("Неверное значение параметра: %1").arg(P_ADDRESS));


      return true;

    }

    catch(SvException& e) {
      _last_error = e.error;
      return false;
    }
  }

private:
  QString _params_string = "";

  DeviceParams _params;

  QString _last_error = "";

  SvException _exception;

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


#endif // SV_DEV_DEFS_H

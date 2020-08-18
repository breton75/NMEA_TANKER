#ifndef SVWEBSERVER_H
#define SVWEBSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QHttpMultiPart>
#include <QDir>
#include <QHash>

#include "../../svlib/sv_abstract_logger.h"
#include "../../svlib/sv_exception.h"

#include "../global/defs.h"
#include "../global/sv_signal.h"

class SvWebServer : public QObject
{
  Q_OBJECT
public:
  explicit SvWebServer(QObject *parent = 0);
  ~SvWebServer();

  bool init(sv::SvAbstractLogger* logger = nullptr, quint16 port = 80);
  void stop();

  void addSignal(SvSignal* signal) throw(SvException)
  {
    if(m_signals_by_id.contains(signal->info()->id))
      throw SvException(QString("Повторяющееся id сигнала: %1").arg(signal->info()->id));

    if(m_signals_by_name.contains(signal->info()->name))
      throw SvException(QString("Повторяющееся id сигнала: %1").arg(signal->info()->name));

    m_signals_by_id.insert(signal->info()->id, signal);
    m_signals_by_name.insert(signal->info()->name, signal);
  }

private:
  QTcpServer m_server;
  sv::SvAbstractLogger* m_logger;
  quint16 m_port = 80;

  QMap<int, QTcpSocket*> m_clients;

  QMap<int, SvSignal*> m_signals_by_id;
  QMap<QString, SvSignal*> m_signals_by_name;

signals:

public slots:

private slots:
  void newConnection();
  void readClient();

};

#endif // SVWEBSERVER_H

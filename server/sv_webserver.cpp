#include "sv_webserver.h"

using namespace sv::log;

SvWebServer::SvWebServer(QObject *parent) :
  QObject(parent)
{

}


SvWebServer::~SvWebServer()
{
  stop();
}

bool SvWebServer::init(sv::SvAbstractLogger *logger, quint16 port)
{
  m_logger = logger;
  m_port = port;


  connect(&m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));

  if (!m_server.listen(QHostAddress::Any, m_port))
  {
    if(m_logger)
      *m_logger << llInfo << mtInfo << TimeZZZ << QString("Web сервер запущен на %1 порту").arg(m_port) << sv::log::endl;

    qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(m_server.errorString());

    return true;

  } else {

    if(m_logger)
      *m_logger << llInfo << mtInfo << TimeZZZ << QString("Web сервер запущен на %1 порту").arg(m_port) << sv::log::endl;

    qDebug() << QString::fromUtf8("Сервер запущен!");

    return false;
  }
}

void SvWebServer::newConnection()
{
  if(m_server.isListening())
  {
    qDebug() << QString::fromUtf8("У нас новое соединение!");

    QTcpSocket* client = m_server.nextPendingConnection();

    m_clients.insert(client->socketDescriptor(), client);
    connect(client, SIGNAL(readyRead()), this, SLOT(readClient()));

  }
}

void SvWebServer::readClient()
{
  // Получаем объект сокета, который вызвал данный слот
  QTcpSocket* client = (QTcpSocket*)sender();

  QFile f("html/index.html");
  if(!f.open(QIODevice::ReadOnly))
  {
    if(m_logger)
      *m_logger <<llError << mtError <<f.errorString() << sv::log::endl;

    f.close();

    return;
  }

  QString html = QString(f.readAll());

  f.close();

//  QString data = "";
//  for(SvSignal* signal: m_signals)
//  {
//    data.append("\r\n");
//    data.append(QString("<tr><td style=\"width: 66.3px;\" \"background-color: #99ccff;\">%1</td>").arg(signal->info()->name));
//    data.append(QString("<td id=\"%1\" style=\"width: 89.7px;\">%2</td></tr>")
//                .arg(signal->info()->id)
//                .arg(signal->value(), 0, 'g', 4));
//  }

  // Пример отправки ответа клиенту
  QTextStream os(client);
  os.setAutoDetectUnicode(true);
  os << "HTTP/1.0 200 Ok\r\n"
     << "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
     << html //.replace("%DATA%", data)
     << QDateTime::currentDateTime().toString() << "\n";

  // Полученные данные от клиента выведем в qDebug,
  // можно разобрать данные например от GET запроса и по условию выдавать необходимый ответ.
  qDebug() << client->readAll() + "\n\r";

  // Если нужно закрыть сокет
//  client->close();

  // Удалим объект сокета из карты
//  m_clients.remove(client->socketDescriptor());

}

void SvWebServer::stop()
{
  foreach(int i, m_clients.keys())
  {
      QTextStream os(m_clients[i]);
      os.setAutoDetectUnicode(true);
      os << QDateTime::currentDateTime().toString() << "\n";

      m_clients[i]->close();
      m_clients.remove(i);
  }

  m_server.close();

  qDebug() << QString::fromUtf8("Сервер остановлен!");

}

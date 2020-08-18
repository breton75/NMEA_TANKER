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

  QByteArray request = client->readAll();

//  QStringList sd = QString(request).split("\r\n");
//  for(QString d: sd) qDebug() << d;

  QList<QByteArray> parts = request.split('\n');
  if((parts.count() < 2) || !(parts.at(0).toUpper().startsWith("GET") || parts.at(0).toUpper().startsWith("POST")))
    return;

  QTextStream replay(client);
  replay.setAutoDetectUnicode(true);

  if(parts.at(0).toUpper().startsWith("GET"))
  {
    QDir dir("html");
    QFile f(dir.absoluteFilePath("index2.html"));

    if(!f.open(QIODevice::ReadOnly))
    {
      if(m_logger)
        *m_logger <<llError << mtError <<f.errorString() << sv::log::endl;


      replay << "HTTP/1.1 500 Error"
             << "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
             << QString("<html>"
                            "<head><meta charset=\"UTF-8\"><title>Ошибка</title>head>"
                            "<body>"
                            "<p \"style=font-size: 14\">%1</p>"
                            "</body></html>").arg(f.errorString())
             << QDateTime::currentDateTime().toString() << "\n";

      f.close();

    }

    else
    {

      QString html = QString(f.readAll());

      f.close();

//qDebug() <<
      replay << "HTTP/1.1 200 Ok\r\n"
         << "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
         << html << "\r\n"
         << QDateTime::currentDateTime().toString() << "\n";

      // Полученные данные от клиента выведем в qDebug,
      // можно разобрать данные например от GET запроса и по условию выдавать необходимый ответ.
//      qDebug() << client->readAll() + "\r\n";

    }
  }
  else if (parts.at(0).toUpper().startsWith("POST"))
  {
    QStringList r1 = QString(parts.last()).split('?');

    if(r1.count() < 2)
      return;

    QString answer = ""; // формируем ответ в формате JSON

    if(r1.at(0) == "names")
    {
      QStringList names = QString(r1.at(1)).split(',');

      for(QString name: names)
      {
        if(name.trimmed().isEmpty())
          continue;
        if(m_signals_by_name.contains(name))
          answer.append(QString("{\"name\": \"%1\", \"value\": \"%2\"},")
                        .arg(name).arg(m_signals_by_name.value(name)->value()));

      }

      if(!answer.isEmpty()) answer.chop(1);
    }

    else if(r1.at(0) == "ids")
    {
      QStringList ids = QString(r1.at(1)).split(',');

      for(QString curid: ids)
      {
        if(curid.trimmed().isEmpty())
          continue;

        bool ok;
        int id = curid.toInt(&ok);

        if(ok && m_signals_by_id.contains(id))
          answer.append(QString("{\"id\": \"%1\", \"value\": \"%2\"}")
                        .arg(id).arg(m_signals_by_id.value(id)->value()));

      }

      if(!answer.isEmpty()) answer.chop(1);
    }

    qDebug() << answer;
    replay << "HTTP/1.0 200 Ok\r\n"
           << "Content-Type: text/plain; charset=\"utf-8\"\r\n"
           << QString("Content-Length: %1\r\n").arg(answer.length() + 2)
           << "Access-Control-Allow-Origin: *\r\n"
           << "Access-Control-Allow-Headers: *\r\n"
           << "Origin: file://\r\n\r\n"
           << "[" << answer << "]\n"; // формируем ответ в формате JSON
//           << QDateTime::currentDateTime().toString() << "\n";

    //           << "Host: 172.16.4.11\r\n"
    //           << "Accept: */*\r\n"
    //           << "Origin: file://\r\n"
  }

//   Если нужно закрыть сокет
  client->close();

//   Удалим объект сокета из карты
  m_clients.remove(client->socketDescriptor());

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

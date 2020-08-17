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

  QList<QByteArray> parts = request.split(' ');
  if(request.length() < 4 || parts.count() < 3)
    return;

  if(!(parts.at(0).toUpper().startsWith("GET") || parts.at(0).toUpper().startsWith("POST")))
    return;


  QTextStream replay(client);
  replay.setAutoDetectUnicode(true);

  if(parts.at(0).toUpper().startsWith("GET"))
  {
    QDir dir("html");
    QFile f(dir.absoluteFilePath("index.html"));

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


      replay << "HTTP/1.1 200 Ok\r\n"
         << "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
         << html
         << QDateTime::currentDateTime().toString() << "\n";

      // Полученные данные от клиента выведем в qDebug,
      // можно разобрать данные например от GET запроса и по условию выдавать необходимый ответ.
      qDebug() << client->readAll() + "\n\r";

    }
  }
  else if (parts.at(0).toUpper().startsWith("POST"))
  {
    QStringList r1 = QString(parts.at(1)).split('?');
    qDebug() << "\n\n" << r1.at(1) << "\n\n";
    if(r1.count() < 2)
      return;

    QStringList ids = QString(r1.at(1)).split(',');

    QString values = "{\n";
    for(QString curid: ids)
    {
      bool ok;
      int id = curid.toInt(&ok);

      if(ok && m_signals.contains(id))
      {
        values.append(QString("\"%1\": %2").arg(m_signals.value(id)->info()->name)
                      .arg(m_signals.value(id)->value()));
      }
      values.append("}");

    }

    qDebug() << values;
    replay << "HTTP/1.1 200 Ok\r\n"
           << "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
           << values
           << QDateTime::currentDateTime().toString() << "\n";
  }

  // Если нужно закрыть сокет
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

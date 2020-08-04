#include <QCoreApplication>
#include <QLibrary>
#include <QDebug>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  QLibrary l("/home/user/nmea/lib/libtestlib.so.1.0.0");

  if(!l.load())
    qDebug() << l.errorString();
  else
    qDebug() << "loaded";



  l.unload();

  return a.exec();
}


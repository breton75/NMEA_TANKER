#ifndef SVSTORAGEWATCHER_H
#define SVSTORAGEWATCHER_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutexLocker>
#include <QTimer>

class SvStorageWatcher : public QThread
{
  Q_OBJECT

public:

  explicit SvStorageWatcher();
  explicit SvStorageWatcher(int interval, QSqlDatabase db );

      int			interval() const;
      void			setInterval( int val );

      bool			isConnected() const;
      QSqlDatabase	connection() const;
      void			setConnection( QSqlDatabase db );

signals:
      void			connected();
      void			disconnected();

protected slots:
      void			reconnect();

protected:
  virtual	void			run();

  int					m_interval;
  QSqlDatabase			m_db;
  QTimer				*m_timer;
  bool					m_connOk;
  mutable QMutex		m_mutex;

};

#endif // SVSTORAGEWATCHER_H

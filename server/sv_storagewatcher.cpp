#include "sv_storagewatcher.h"

SvStorageWatcher::SvStorageWatcher() :
  QThread( 0 ),
  m_interval( 0 ),
  m_db(),
  m_timer( 0 ),
  m_connOk( false )
{
  moveToThread( this );
}

SvStorageWatcher::SvStorageWatcher( int interval, QSqlDatabase db ) :
  QThread( 0 ),
  m_interval( interval ),
  m_db( db ),
  m_timer( 0 ),
  m_connOk( false )
{
  moveToThread( this );
}

int SvStorageWatcher::interval() const
{
  QMutexLocker lock( &m_mutex );
  return m_interval;
}

void SvStorageWatcher::setInterval( int val )
{
  QMutexLocker lock( &m_mutex );
  m_interval = val;
}

bool SvStorageWatcher::isConnected() const
{
  QMutexLocker lock( &m_mutex );
  return m_db.isOpen() && m_db.exec( "SELECT TRUE" ).isActive();
}

QSqlDatabase SvStorageWatcher::connection() const
{
  QMutexLocker lock( &m_mutex );
  return m_db;
}

void SvStorageWatcher::setConnection( QSqlDatabase db )
{
  QMutexLocker lock( &m_mutex );
  m_db = db;
}

void SvStorageWatcher::reconnect()
{
  Q_ASSERT( m_timer );

  if( isConnected() )
    return;

  m_timer->stop();

  if( m_connOk )
  {
    m_connOk = false;
    emit disconnected();
  }

  m_db.open();

  if( isConnected() )
  {
    m_connOk = true;
    emit connected();
  }

  m_timer->start();
}

void SvStorageWatcher::run()
{
  m_timer = new QTimer;
  connect( m_timer, SIGNAL(timeout()), SLOT(reconnect()) );
  m_timer->setSingleShot( false );
  m_timer->setInterval( m_interval * 1000 );
  m_timer->start();

  QMetaObject::invokeMethod( this, "reconnect", Qt::QueuedConnection );

  exec();

  m_timer->stop();
  delete m_timer;
}

#include "sv_signal.h"

SvSignal::SvSignal(SignalParams& params)
{
  setParams(params);
  
  // добавляем репозитории
//  _storages.clear();
  
//  QStringList l = params.storages.split(',');
  
//  for(QString r: l) {
    
//    if(r.isEmpty()) continue;
    
//    bool ok;
//    int id = r.toInt(&ok);
    
//    if(ok) _storages.append(id);
    
//  }
  
//  connect(&_timer, &QTimer::timeout, this, &SvSignal::timeout);
  
}

SvSignal::~SvSignal()
{
  deleteLater();
}

void SvSignal::setValue(qreal value)
{
  _previous_value = _value;
  _value = value;
  _lost_epoch = QDateTime::currentDateTime().toMSecsSinceEpoch() + _params.timeout;
}


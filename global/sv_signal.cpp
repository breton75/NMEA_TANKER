#include "sv_signal.h"

SvSignal::SvSignal(SignalInfo &info)
{
  setup(info);
}

SvSignal::~SvSignal()
{
  deleteLater();
}

void SvSignal::setValue(qreal value)
{
  p_previous_value = p_value;
  p_value = value;
  p_last_update = QDateTime::currentDateTime();
  p_lost_epoch = p_last_update.toMSecsSinceEpoch() + p_info.timeout;
}


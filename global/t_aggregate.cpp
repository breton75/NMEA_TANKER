#include "t_aggregate.h"

TAggregate::TAggregate(THost host, quint16 interval, States state, TAggStateLabel* label):
    _host(host), _interval(interval), _state(state), _label(label)
{
    _timer.setInterval(_interval);

    set_state(_state);

}

TAggregate::~TAggregate()
{
    _timer.stop();
    delete _label;
}

void TAggregate::start()
{
//    connect(&_timer, SIGNAL(timeout()), this, SLOT(update_state()));
//    _timer.start();
}

void TAggregate::set_state(States state)
{
    _state = state;

    _label->lblState->setText(states_text.value(state));

    int red = states_color.value(state).red();
    int green = states_color.value(state).green();
    int blue = states_color.value(state).blue();
    _label->lblState->setStyleSheet(QString("color:rgb(%1,%2,%3)").arg(red).arg(green).arg(blue));
}

void TAggregate::update_state()
{
    QTcpSocket sock;
    sock.connectToHost(QHostAddress(_host.addr), _host.port);

    if(sock.waitForConnected(100)) {

      set_state(States::Available);
      sock.disconnectFromHost();

    }
    else {

      set_state(States::Unreachable);

    }
}

States TAggregate::force_update_state()
{
    set_state(States::Unknown);
    update_state();
    return _state;
}

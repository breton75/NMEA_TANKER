#ifndef TAggregate_H
#define TAggregate_H

#include <QObject>
#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QColor>
#include <QMap>
#include <QTimer>

#include "t_agg_state_label.h"

#define STATE_UNKNOWN "--"
#define STATE_AVAILABLE "Доступен"
#define STATE_UNREACHABLE "Не доступен"
#define STATE_WRONGHOST "Неверный хост"
#define STATE_USING "Используется"


enum States {
    Unknown,
    Available,
    Unreachable,
    WrongHostname,
    Using
};

const QMap<States, QString> states_text = {{States::Unknown, QString(STATE_UNKNOWN)},
                                           {States::Available, QString(STATE_AVAILABLE)},
                                           {States::Unreachable, QString(STATE_UNREACHABLE)},
                                           {States::WrongHostname, QString(STATE_WRONGHOST)},
                                           {States::Using, QString(STATE_USING)}};

const QMap<States, QColor> states_color =   {{States::Unknown, QColor(0,0,0)},
                                           {States::Available, QColor(0,0,204)},
                                           {States::Unreachable, QColor(255,50,50)},
                                           {States::WrongHostname, QColor(50,50,50)},
                                           {States::Using, QColor(0,204,0)}};

struct THost
{
    explicit THost(quint32 a, quint16 p):
        addr(a), port(p)
    { }

    quint32 addr;
    quint16 port;
};

class TAggregate: public QObject
{
    Q_OBJECT

public:
    explicit TAggregate(THost host, quint16 interval, States state, TAggStateLabel* label);

    ~TAggregate();

    const THost& host() const { return _host; }
    const States state() const { return _state; }
    const QLabel* label() const { return _label->lblState; }

    void start();

    void set_state(States state);
    States force_update_state();

private slots:
    void update_state();

private:
    QTimer _timer;
    THost _host;
    quint16 _interval;
    States _state;
    TAggStateLabel* _label = nullptr;

};


#endif // TAggregate_H

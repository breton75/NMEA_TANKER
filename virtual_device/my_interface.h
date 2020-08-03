#ifndef MY_INTERFACE
#define MY_INTERFACE

#include <QtCore>

enum AvailableIfces {
  RS,
  RS485,
  UDP
};

const QMap<QString, AvailableIfces> ifcesMap = {{"RS",    AvailableIfces::RS},
                                                {"RS485", AvailableIfces::RS485},
                                                {"UDP",   AvailableIfces::UDP}};



#endif // MY_INTERFACE


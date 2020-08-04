#ifndef ABSTRACT_TEST
#define ABSTRACT_TEST

#include <QtCore>

class AbstractTest: public QObject
{
  Q_OBJECT

public:
  AbstractTest() { }

protected:

  virtual QString foo() = 0;

};

#endif // ABSTRACT_TEST


#ifndef OBSERVER_H
#define OBSERVER_H

#include "qvariant.h"
class Observer {
 public:
  virtual ~Observer(){};
  virtual void Update(const QString &event, const QVariant &value) = 0;
  virtual void Update(const QString &event) = 0;
};

#endif  // OBSERVER_H

#ifndef PUBLISHER_H
#define PUBLISHER_H

class Publisher {
 public:
  virtual ~Publisher(){};
  virtual void Attach(Observer *observer) = 0;
  virtual void Detach(Observer *observer) = 0;
  virtual void Notify(const QString &event) = 0;
  virtual void Notify(const QString &event, const QVariant &value) = 0;
};

#endif  // PUBLISHER_H

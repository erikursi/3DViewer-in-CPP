#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QDebug>
#include <QMap>
#include <memory>
#include <mutex>

#include "publisher.h"
#include "qobject.h"
#include "qvariant.h"
class Mediator : public Publisher {
 public:
  // Static method to get the instance of the singleton
  static Mediator &getInstance() {
    std::call_once(initInstanceFlag, &Mediator::initSingleton);
    return *instance;
  }

  // Destructor (optional)
  ~Mediator() = default;

  void Notify(const QString &event) override {
    std::list<Observer *>::iterator iterator = list_observer_.begin();
    while (iterator != list_observer_.end()) {
      (*iterator)->Update(event);
      ++iterator;
    }
  }
  void Notify(const QString &event, const QVariant &value) override {
    std::list<Observer *>::iterator iterator = list_observer_.begin();
    while (iterator != list_observer_.end()) {
      (*iterator)->Update(event, value);
      ++iterator;
    }
  }

  void Attach(Observer *observer) override {
    list_observer_.push_back(observer);
  }
  void Detach(Observer *observer) override { list_observer_.remove(observer); }

  // Delete copy constructor and assignment operator to ensure singleton
  Mediator(const Mediator &) = delete;
  Mediator &operator=(const Mediator &) = delete;

 private:
  std::list<Observer *> list_observer_;

  // Private constructor to prevent instantiation
  Mediator() = default;

  // Method to initialize the singleton instance
  static void initSingleton() { instance.reset(new Mediator()); }

  // Static unique_ptr to hold the singleton instance
  static std::unique_ptr<Mediator> instance;
  // Flag to ensure the instance is initialized only once
  static std::once_flag initInstanceFlag;

  // Mutex for thread safety
  std::mutex mutex;
};

#endif  // MEDIATOR_H

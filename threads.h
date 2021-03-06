#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QDebug>

#include "top-fifteen.h"

// second variant
class Handler : public QObject
{
  Q_OBJECT

public:
  explicit Handler(QObject* parent = nullptr) : QObject(parent)
  {

  }


//public:
  //TopFifteen tops;
};



template <typename TWorker>
class Thread : public QThread
{
public:
  explicit Thread(TWorker* worker, QObject* parent = nullptr)
    : QThread(parent), _worker(worker)
  {
    _worker->moveToThread(this);
    start();
  }

  ~Thread()
  {
    quit();
    wait();
  }

  TWorker* worker()
  {
    return _worker;
  }

protected:
  void run() override
  {
    QThread::run();
    delete _worker;
  }

private:
  TWorker* _worker;
};

using TWorker = Thread<TopFifteen>;
// interface
class HandlerService : public TWorker
{
  Q_OBJECT

public:
  explicit HandlerService(QObject* parent = nullptr);

signals:
  void handleFile(const QString& filename);
};


#endif // LOGGER_H

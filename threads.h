#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QDebug>

//#include <QtConcurrent>

// second variant
// the worker that will be moved to a thread
class LogWorker : public QObject
{
  Q_OBJECT

public:
  explicit LogWorker(QObject* parent = nullptr) : QObject(parent) {}


public slots:
  // this slot will be executed by event loop (one call at a time)
  void logEvent(const QString& event)
  {
    qDebug() << QThread::currentThreadId() << "  ";
    qDebug() << event;
  }
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

using TWorkerLog = Thread<LogWorker>;
// interface
class LogService : public TWorkerLog//public Thread<LogWorker>
{
  Q_OBJECT

public:
  explicit LogService(QObject* parent = nullptr);

signals:
  void logEvent(const QString& event);
};


#endif // LOGGER_H

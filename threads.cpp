#include "threads.h"

LogService::LogService(QObject* parent)
  : TWorkerLog(new LogWorker, parent)
{
  const LogWorker* p_worker = worker();
  connect(this, &LogService::logEvent, p_worker, &LogWorker::logEvent);
}

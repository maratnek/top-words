#include "threads.h"

HandlerService::HandlerService(QObject* parent)
  : TWorker(new Handler, parent)
{
  connect(this, &HandlerService::handleFile, worker(), &Handler::handleFile);
}

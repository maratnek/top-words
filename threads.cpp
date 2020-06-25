#include "threads.h"
#include "top-fifteen.h"

HandlerService::HandlerService(QObject* parent)
  : TWorker(new TopFifteen, parent)
{
  connect(this, &HandlerService::handleFile, worker(), &TopFifteen::handleFile);
}

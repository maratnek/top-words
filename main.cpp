#include <iostream>
#include <QApplication>
#include "plotter.h"

int main(int ac, char** av)
{
  QApplication app(ac, av);
  Plotter plot;
  plot.show();
  return app.exec();
}
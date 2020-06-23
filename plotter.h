#ifndef PLOTTER_H
#define PLOTTER_H
#include <QMainWindow>
//#include "top-fifteen.h"
#include "threads.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class Plotter;
}
QT_END_NAMESPACE

class Plotter : public QMainWindow
{
  Q_OBJECT

public:
  explicit Plotter(QWidget* parent = nullptr);
  ~Plotter();

private slots:

  void open();

  void about();

  void exit();

  void update();

private:
  Ui::Plotter* ui;
  QString currentFile;
  HandlerService handlerServ;
};

#endif // Plotter_H
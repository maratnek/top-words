#ifndef PLOTTER_H
#define PLOTTER_H
#include <QMainWindow>
//#include "top-fifteen.h"
#include "threads.h"

// qchart
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts>

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

  void updateChart();

  void progressBarUpdate(quint8 value);


private:
  Ui::Plotter* ui;
  QString currentFile;
  HandlerService* m_handlerServ;

  QChart* m_chart;
  QHorizontalStackedBarSeries* m_series;
  QBarSet* m_barset;
  QBarCategoryAxis* m_axisY;

  void initChart();

//timer
  QTimer* m_timer;
};

#endif // Plotter_H

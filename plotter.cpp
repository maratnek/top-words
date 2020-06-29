#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <QRegExp>

#include "plotter.h"
#include "ui_Plotter.h"

constexpr uint chart_update_time = 2000;

Plotter::Plotter(QWidget* parent) :
  QMainWindow(parent),
  m_handlerServ(new HandlerService(parent)),
  m_timer(new QTimer(parent)),
  m_barset(new QBarSet("")),
  m_chart(new QChart),
  ui(new Ui::Plotter)
{
  ui->setupUi(this);

  connect(ui->actionOpen, &QAction::triggered, this, &Plotter::open);

  connect(ui->actionExit, &QAction::triggered, this, &Plotter::exit);

  connect(ui->actionAbout, &QAction::triggered, this, &Plotter::about);


  this->initChart();

  for (size_t i = 0; i < max_tops_count; i++)
  {
    ui->listWidget->addItem("");
  }

  ui->progressBar->hide();
  ui->listWidget->hide();
  ui->listWidget->setMinimumWidth(400);

  connect(m_handlerServ->worker(), &TopFifteen::progressSend, this, &Plotter::progressBarUpdate);
  connect(m_handlerServ->worker(), &TopFifteen::finishHandle, this, [ = ]()
  {
    m_timer->stop();
    this->updateChart();
    this->update();
    // statistic data
    ui->statusBar->showMessage("Status bar: matched words -> " + QString::number(m_handlerServ->worker()->m_max_matched_words));
  });

  connect(m_timer, &QTimer::timeout, this, [ = ]()
  {
    this->updateChart();
  });

}

void Plotter::progressBarUpdate(quint8 value)
{
  ui->progressBar->show();
  ui->progressBar->setValue(value);
  this->update();
}

void Plotter::updateChart()
{
  auto handler = m_handlerServ->worker();
  auto it = handler->m_vtops.begin();

  QBarCategoryAxis* axisY = static_cast<QBarCategoryAxis*>(m_chart->axisY());
  axisY->clear();
  int max = max_tops_count;

  qSort(handler->m_vtops.begin(), handler->m_vtops.end(), [](QPair<QString, int> l, QPair<QString, int> r)
  {
    return l.first > r.first;
  });

  for (int i = 0; i < handler->m_vtops.size() && i < max_tops_count; ++i)
  {
    max = it->second > max ? it->second : max;
    m_barset->replace(i, it->second);
    axisY->append(it->first);
    it++;
  }

  QValueAxis* axisX = static_cast<QValueAxis*>(m_chart->axisX());

  axisX->setMax(max);

  m_chartView->show();
}

void Plotter::update()
{
  ui->listWidget->show();
  ui->listWidget->setSortingEnabled(false);
  auto handler = m_handlerServ->worker();
  auto it = handler->m_vtops.begin();

  for (int i = 0; i < max_tops_count && it != handler->m_vtops.end(); ++i)
  {
    ui->listWidget->item(i)->setText(it->first + " -> " + QString::number(it->second));
    ++it;
  }

  ui->listWidget->setSortingEnabled(true);

  ui->listWidget->sortItems(Qt::AscendingOrder);
}

Plotter::~Plotter()
{
  delete ui;
}

void Plotter::open()
{
  QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
  QFile file(fileName);
  currentFile = fileName;

  if (!file.open(QIODevice::ReadOnly | QFile::Text))
  {
    QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
    return;
  }

  setWindowTitle(fileName);

  m_handlerServ->handleFile(fileName);

  m_timer->start(chart_update_time);
}

void Plotter::exit()
{
  QCoreApplication::quit();
}

void Plotter::about()
{
  QMessageBox::about(this, tr("About MDI"),
                     tr("The <b>Plotter</b> example demonstrates how to code a basic "
                        "text editor using QtWidgets"));

}

void Plotter::initChart()
{

  QStringList categories;

  for (size_t i = 0; i < max_tops_count; i++)
  {
    *m_barset << i;
    categories << "item: " + QString::number(i);
  }

  QHorizontalStackedBarSeries* series = new QHorizontalStackedBarSeries();
  series->append(m_barset);

  m_chart->addSeries(series);
  m_chart->setTitle("Top fifteen chart");

  QBarCategoryAxis* axisY = new QBarCategoryAxis();
  axisY->append(categories);
  m_chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);
  QValueAxis* axisX = new QValueAxis();
  axisX->setLabelFormat("%d");
  m_chart->addAxis(axisX, Qt::AlignBottom);

  series->attachAxis(axisX);

  m_chart->legend()->setVisible(true);
  m_chart->legend()->setAlignment(Qt::AlignBottom);
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  m_chartView = new QChartView(m_chart);
  m_chartView->setRenderHint(QPainter::Antialiasing);

  m_chartView->setMinimumWidth(800);

  ui->horizontalLayout->addWidget(m_chartView);
  m_chartView->hide();

}

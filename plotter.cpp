#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <QRegExp>

#include "plotter.h"
#include "ui_Plotter.h"

Plotter::Plotter(QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::Plotter)
{
  ui->setupUi(this);

  connect(ui->actionOpen, &QAction::triggered, this, &Plotter::open);

  connect(ui->actionExit, &QAction::triggered, this, &Plotter::exit);

  connect(ui->actionAbout, &QAction::triggered, this, &Plotter::about);

  for (size_t i = 0; i < max_tops_count; i++)
  {
    ui->listWidget->addItem("");
  }

  ui->progressBar->hide();

  connect(handlerServ.worker(), &Handler::finishHandle, this, &Plotter::update);
  connect(&handlerServ.worker()->tops, &TopFifteen::progressSend, this, &Plotter::progressBarUpdate);

}

void Plotter::progressBarUpdate(quint8 value)
{
  ui->progressBar->show();
  ui->progressBar->setValue(value);
}

void Plotter::update()
{
  auto handler = handlerServ.worker();
  QMap<QString, int>::iterator it = handler->tops.m_tops.begin();

  ui->listWidget->sortItems(Qt::AscendingOrder);

  for (int i = 0; i < ui->listWidget->count(); ++i)
  {
    ui->listWidget->item(i)->setText(it.key() + " -> " + QString::number(it.value()));
    qDebug() << i << it.key() + " -> " + QString::number(it.value()) << '\n';
    it++;
    ui->listWidget->update();
  }

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

  handlerServ.handleFile(fileName);

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

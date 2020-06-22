#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <QRegExp>

#include "plotter.h"
#include "ui_Plotter.h"

//std
#include <queue>
#include <vector>
// regular exp word
constexpr char* regular_const = "([^\\d,^\\W]+\\w+)";
// (\\w+)

#include "threads.h"

Plotter::Plotter(QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::Plotter)
{
  ui->setupUi(this);

  connect(ui->actionOpen, &QAction::triggered, this, &Plotter::open);

  connect(ui->actionExit, &QAction::triggered, this, &Plotter::exit);

  connect(ui->actionAbout, &QAction::triggered, this, &Plotter::about);



}

Plotter::~Plotter()
{
  delete ui;
}

#include <queue>
#include <QPair>
#include <iostream>

auto cmp = [](auto left, auto right)
{
  return (left.second) > (right.second);
};
using PriorQueue = std::priority_queue < std::pair<QString, int>, std::vector<std::pair<QString, int>>, decltype(cmp)>;
template<typename T> void print_queue(T& q)
{
  T t = q;

  while (!t.empty())
  {
    qDebug() << t.top() << " ";
    t.pop();
  }

  std::cout << '\n';
}

class TopFifteen
{
  const int topCount = 15;
  int count = 0;
  using TMapIter = QMap<QString, int>::iterator;
public:
  TopFifteen() : m_min(0), m_max(0), queueTops(cmp)
  {
  }

  void addElement(const std::pair<QString, int>& elem)
  {

    if (m_tops.count() > 15)
    {
      if (m_tops.contains(elem.first))
      {
        m_tops[elem.first] = elem.second;
      }
      else
      {
        auto iter = std::min(m_tops.begin(), m_tops.end(), [](TMapIter l, TMapIter r)
        {
          return l.value() < r.value();
        });

        if (iter != m_tops.end() && iter.value() < elem.second)
        {
          m_tops.remove(iter.key());
          m_tops[elem.first] = elem.second;
        }

      }
    }
    else
    {
      m_tops.insert(elem.first, elem.second);
    }
  }

  void addElement2(const std::pair<QString, int>& elem)
  {
    PriorQueue temp(cmp);

    bool exist_elem = false;

    while (!queueTops.empty())
    {
      auto it = queueTops.top();

      if (it.first != elem.first)
      {
        temp.push(it);
      }
      else
      {
        exist_elem = true;
      }

      queueTops.pop();
    }

    if (exist_elem)
    {
      while (!temp.empty())
      {
        queueTops.push(temp.top());
        temp.pop();
      }
    }

    print_queue(queueTops);

    queueTops.push(elem);

    if (++count > topCount)
    {
      queueTops.pop();
    }

  }

  ~TopFifteen()
  {
  }

  void handleFile(QString fileName);

  int m_max, m_min;
  PriorQueue queueTops;
  QMap<QString, int> m_tops;

};

TopFifteen tops;


#include <QHash>

QHash<QString, int> hashCount;

void TopFifteen::handleFile(QString fileName)
{
  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly | QFile::Text))
  {
    QMessageBox::warning(0, "Warning", "Cannot open file: " + file.errorString());
    return;
  }

  qDebug() << "Handle file" << '\n';
  QTextStream in(&file);
  QRegExp rx(regular_const);

  while (!in.atEnd())
  {
    QString line = in.readLine();
    QStringList list;
    int pos = 0;

    while ((pos = rx.indexIn(line, pos)) != -1)
    {
      list << rx.cap(1);
      pos += rx.matchedLength();
    }

    for (auto it : list)
    {
      auto count = hashCount[it];
      hashCount.insert(it, ++count);
      tops.addElement({ it, count });
    }

    qDebug() << list << '\n';
  }

  auto i = hashCount.constBegin();

  while (i != hashCount.constEnd())
  {
    qDebug() << i.key() << ": " << i.value() << '\n';
    ++i;
  }
}

#include <QThread>
#include <qtconcurrentrun.h>
using namespace QtConcurrent;

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

  //handleFile(file);
  LogService logServ(this);
  qDebug() << "send from log service";
  logServ.logEvent("Log Event show");
  logServ.logEvent("Log Event show");
  //QFuture<void> f1 = run(handleFile, fileName);
  //f1.waitForFinished();

  //while (!tops.queueTops.empty())
  //{
  //  auto elem = tops.queueTops.top();
  //  ui->listWidget->addItem(elem.first + " " + QString::number(elem.second));
  //  tops.queueTops.pop();
  //}

  QMap<QString, int>::iterator it = tops.m_tops.begin();

  while (it != tops.m_tops.constEnd())
  {
    ui->listWidget->addItem(it.key() + " -> " + QString::number(it.value()));
    ++it;
  }



  //QTextStream in(&file);
  //QString text = in.readAll();
  //ui->textEdit->setText(text);
  //file.close();
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

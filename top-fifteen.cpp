#include "top-fifteen.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <QRegExp>


using TMapIter = QMap<QString, int>::iterator;

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
      this->addElement({ it, count });
    }

    //qDebug() << list << '\n';
  }

  //auto i = hashCount.constBegin();

  //while (i != hashCount.constEnd())
  //{
  //  qDebug() << i.key() << ": " << i.value() << '\n';
  //  ++i;
  //}
}

void TopFifteen::addElement(const std::pair<QString, int>& elem)
{

  if (m_tops.count() > max_tops_count)
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


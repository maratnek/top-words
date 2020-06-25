#include "top-fifteen.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <QRegExp>

#include <QThread>
#include <QDebug>


using TMapIter = QMap<QString, int>::iterator;

void TopFifteen::handleFile(const QString& fileName)
{
  qDebug() << QThread::currentThreadId() << "  ";
  qDebug() << fileName;
  m_tops.clear();
  m_hashCount.clear();

  QFile file(fileName);

  if (!file.open(QIODevice::ReadOnly | QFile::Text))
  {
    QMessageBox::warning(0, "Warning", "Cannot open file: " + file.errorString());
    return;
  }

  auto file_size = file.size();
  quint64 handle_size = 0;
  qDebug() << "Handle file" << fileName << "- size: " << file_size << '\n';

  QTextStream in(&file);
  QRegExp rx(regular_const);
  //QString last_words = "";
  uint sent_max = 15000;
  uint sent_count = 0;

  while (!in.atEnd())
  {
    QString line = in.readLine();

    //QStringList list;
    int pos = 0;
    int prev_pos = 0;

    while ((pos = rx.indexIn(line, pos)) != -1)
    {
      auto word = rx.cap(1);
      auto count = m_hashCount[word];
      m_hashCount.insert(word, ++count);
      this->addElement({ word, count });

      // send event
      if (++sent_count > sent_max)
      {
        sent_count = 0;
        handle_size += pos - prev_pos;
        quint8 progress = double(handle_size) / double(file_size) * 100;
        progressSend(progress);
      }

      pos += rx.matchedLength();
      prev_pos = pos;

      ++m_max_matched_words;
    }

    //qDebug() << "Last index: " << prev_pos << ":" << line.mid(prev_pos);

    // need to emit to the progress bar
    handle_size += line.size() + 2;
    quint8 progress = double(handle_size) / double(file_size) * 100;
    //qDebug() << "Progress: " << progress << "%" << " handle: " << handle_size << '\n';
    progressSend(progress);

    //qDebug() << list << '\n';
  }

  handle_size += 3;
  quint8 progress = 100;
  //qDebug() << "Progress: " << progress << "%" << " handle: " << handle_size << '\n';
  progressSend(progress);

  //auto i = hashCount.constBegin();

  //while (i != hashCount.constEnd())
  //{
  //  qDebug() << i.key() << ": " << i.value() << '\n';
  //  ++i;
  //}
  emit finishHandle();
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


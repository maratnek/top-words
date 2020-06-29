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
  m_hashCount.clear();
  m_vtops.clear();

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
  uint sent_max = 15000;
  uint sent_count = 0;

  while (!in.atEnd())
  {
    QString line = in.readLine();
    int pos = 0;
    int prev_pos = 0;

    while ((pos = rx.indexIn(line, pos)) != -1)
    {
      auto word = rx.cap(1);
      auto count = m_hashCount[word.toLower()];
      m_hashCount.insert(word.toLower(), ++count);
      this->addElement({ word.toLower(), count });

      // send event
      if (++sent_count > sent_max)
      {
        sent_count = 0;
        handle_size += pos - prev_pos;
        quint8 progress = double(handle_size) / double(file_size) * 100;
        emit progressSend(progress);
      }

      pos += rx.matchedLength();
      prev_pos = pos;

      ++m_max_matched_words;
    }

    handle_size += line.size() + 2;
    quint8 progress = double(handle_size) / double(file_size) * 100;
    emit progressSend(progress);
  }

  quint8 progress = 100;
  emit progressSend(progress);

  emit finishHandle();
}

void TopFifteen::addElement(const QPair<QString, int>& elem)
{
  for (auto& it : m_vtops)
  {
    if (it.first == elem.first)
    {
      it.second = elem.second;
      return;
    }
  }

  m_vtops.push_back(elem);
  std::sort(m_vtops.begin(), m_vtops.end(), [](auto l, auto r)
  {
    return l.second > r.second;
  });

  if (m_vtops.size() > max_tops_count)
  {
    m_vtops.pop_back();
  }

}


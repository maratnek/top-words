#ifndef TOP_FIFTEEN_H
#define TOP_FIFTEEN_H
#include <QMainWindow>
#include <QHash>
#include <QMap>

#include "top-fifteen.h"

// regular exp word
constexpr char* regular_const = "([^\\d,^\\W]+\\w+)";

constexpr int max_tops_count = 15;

class TopFifteen : public  QObject
{
  Q_OBJECT

  int count = 0;

  QHash<QString, int> m_hashCount;

signals:
  void progressSend(quint8);

signals:
  void finishHandle();

public slots:
  void handleFile(const QString& fileName);

public:
  TopFifteen(QObject* parent = nullptr) : QObject(parent) {}
  ~TopFifteen() {}

  void addElement(const std::pair<QString, int>& elem);

  QMap<QString, int> m_tops;
  uint m_max_matched_words = 0;

};
#endif // TOP_FIFTEEN_H

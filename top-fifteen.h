#ifndef TOP_FIFTEEN_H
#define TOP_FIFTEEN_H
#include <QMainWindow>
#include <QHash>
#include <QMap>

#include "top-fifteen.h"

// regular exp word
constexpr char* regular_const = "([^\\d,^\\W]+\\w+)";

constexpr int max_tops_count = 15;

class TopFifteen
{
  int count = 0;

  QHash<QString, int> hashCount;
public:
  TopFifteen()  {}
  ~TopFifteen() {}

  void addElement(const std::pair<QString, int>& elem);

  void handleFile(QString fileName);

  QMap<QString, int> m_tops;

};
#endif // TOP_FIFTEEN_H
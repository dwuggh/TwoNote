#ifndef TWONOTE_CONFIG_H
#define TWONOTE_CONFIG_H

#include <QString>
#include <QDir>
#include <QSizeF>
#include <QDebug>
#include <QColor>

struct PageView {
    QSizeF pageSize;
    int    verticalMargin;
    int    horizontalMargin;
    QColor backgroundColor;
    QColor pageColor;
    QColor defaultPenColor;
    qreal  defaultPenWidth;
};

class Configs {
public:
  Configs();
  QDir tempDir;
  QDir baseDir;
  QSizeF pageSize;
  PageView pageView;
};

extern Configs config;

#endif

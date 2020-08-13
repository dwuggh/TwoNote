#pragma once

#include <QColor>
#include <QDebug>
#include <QDir>
#include <QSizeF>
#include <QString>

struct PageView {
    QSizeF pageSize;
    int verticalMargin;
    int horizontalMargin;
    QColor backgroundColor;
    QColor pageColor;
    QColor defaultPenColor;
    qreal defaultPenWidth;
};

class Configs {
  public:
    Configs();
    QDir tempDir;
    QDir baseDir;
    // QSizeF pageSize;
    PageView pageView;

    friend QDebug operator<<(QDebug argument, const Configs& obj);
};

extern Configs config;

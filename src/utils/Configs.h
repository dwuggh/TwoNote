#ifndef TWONOTE_CONFIG_H
#define TWONOTE_CONFIG_H

#include <QString>
#include <QDir>
#include <QSizeF>
#include <QDebug>

class Configs {
public:
    Configs();
    QDir tempDir;
    QDir baseDir;
    QSizeF pageSize;
};

extern Configs config;

#endif

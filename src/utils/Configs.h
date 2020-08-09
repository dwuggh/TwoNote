#ifndef TWONOTE_CONFIG_H
#define TWONOTE_CONFIG_H

#include <QString>
#include <QDir>
#include <QDebug>

class Configs {
public:
    Configs();
    QDir tempDir;
    QDir baseDir;
};

extern Configs config;

#endif

#include "Configs.h"


Configs::Configs() {
    // directories
    tempDir = QDir(QDir::tempPath());
    tempDir.mkdir("TwoNote");
    tempDir.cd("./TwoNote/");
    baseDir = QDir::home();
    baseDir.mkdir("TwoNote");
    baseDir.cd("./TwoNote/");

    // default page size
    pageSize = QSizeF(1920, 3000);

    pageView = PageView{
        .pageSize         = QSizeF(1920, 3000),
        .verticalMargin   = 50,
        .horizontalMargin = 50,
        .backgroundColor  = QColor(Qt::gray),
        .pageColor        = QColor(Qt::white),
        .defaultPenColor  = QColor(Qt::black),
        .defaultPenWidth  = 3.0,
    };

    qDebug() << "config:"
	     << "base directory: " << baseDir
	     << "temp directory: " << tempDir
	     << "\n";
}


Configs config = Configs();

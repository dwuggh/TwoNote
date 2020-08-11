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
    // pageSize = QSizeF(1920, 3000);

    pageView = PageView{
        .pageSize         = QSizeF(1920, 3000),
        .verticalMargin   = 50,
        .horizontalMargin = 50,
        .backgroundColor  = QColor(Qt::gray),
        .pageColor        = QColor(Qt::white),
        .defaultPenColor  = QColor(Qt::black),
        .defaultPenWidth  = 3.0,
    };

    qDebug() << *this;
}

QDebug operator<<(QDebug argument, const Configs &obj) {
    argument.nospace() << "config:"
		       << "base directory: " << obj.baseDir
		       << "temp directory: " << obj.tempDir << "\n"
		       << "pages view config: " << obj.pageView.pageSize;
    return argument.space();
}

Configs config = Configs();

#include "Configs.h"


Configs::Configs() {
    tempDir = QDir(QDir::tempPath());
    tempDir.mkdir("TwoNote");
    tempDir.cd("./TwoNote/");
    baseDir = QDir::home();
    baseDir.mkdir("TwoNote");
    baseDir.cd("./TwoNote/");
    qDebug() << "config:"
	     << "base directory: " << baseDir
	     << "temp directory: " << tempDir
	     << "\n";
}

#include "mainwindow.h"

#include <KAboutData>
#include <KLocalizedString>
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  KAboutData aboutData(
      // The program name used internally. (componentName)
      QStringLiteral("TwoNote"),
      // A displayable program name string. (displayName)
      i18n("TwoNote"),
      // The program version string. (version)
      QStringLiteral("0.0.1"),
      // Short description of what the app does. (shortDescription)
      i18n("whatever"),
      // The license this code is released under
      KAboutLicense::GPL,
      // Copyright Statement (copyrightStatement = QString())
      i18n("(c) 2015"),
      // Optional text shown in the About box.
      // Can contain any information desired. (otherText)
      i18n("Some text..."),
      // The program homepage string. (homePageAddress = QString())
      QStringLiteral("http://example.com/"),
      // The bug report email address
      // (bugsEmailAddress = QLatin1String("submit@bugs.kde.org")
      QStringLiteral("submit@bugs.kde.org"));
  aboutData.addAuthor(i18n("someone"), i18n("sometask"),
                      QStringLiteral("dwuggh@gmail.com"),
                      QStringLiteral("https://www.google.com"),
                      QStringLiteral("OSC Username"));
  KAboutData::setApplicationData(aboutData);
  MainWindow w;
  w.show();
  return a.exec();
}

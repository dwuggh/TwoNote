cmake_minimum_required(VERSION 3.5)

project(TwoNote LANGUAGES CXX)


# set(QT_MIN_VERSION "5.3.0")
# set(KF5_MIN_VERSION "5.2.0")


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find_package(ECM REQUIRED NO_MODULE)
# set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


find_package(Qt5 COMPONENTS Widgets REQUIRED)
# include(KDEInstallDirs)
# include(KDECMakeSettings)
# include(KDECompilerSettings NO_POLICY_SCOPE)
# include(FeatureSummary)

# Find Qt modules
find_package(Qt5 CONFIG REQUIRED COMPONENTS
  Core    # QCommandLineParser, QStringLiteral
  Widgets # QApplication
  )

# Find KDE modules
# find_package(KF5 REQUIRED COMPONENTS
#   CoreAddons      # KAboutData
#   I18n            # KLocalizedString
#   WidgetsAddons   # KMessageBox
#   )




unset (TwoNote_SOURCES)
file (GLOB_RECURSE TwoNote_SOURCES
  src/utils/*.h
  src/utils/*.cpp
  src/canvas/items/TLineItem.h
  src/canvas/items/TLineItem.cpp
  src/canvas/items/TPixmapItem.h
  src/canvas/items/TPixmapItem.cpp
  src/canvas/TCanvas.h
  src/canvas/TCanvas.cpp
  src/canvas/TView.h
  src/canvas/TView.cpp
  src/canvas/TPage.h
  src/canvas/TPage.cpp
  src/canvas/State.h
  src/canvas/State.cpp
  src/mainwindow.ui
  src/mainwindow.h
  src/mainwindow.cpp
  src/main.cpp
  )


set(QT_LIBRARY
  Qt5::Widgets
  Qt5::Core
  # KF5::CoreAddons
  # KF5::I18n
  # KF5::WidgetsAddons
  )

add_executable(${PROJECT_NAME}
  ${TwoNote_SOURCES}
  )

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBRARY})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
  ${CMAKE_CURRENT_SOURCE_DIR}/src/canvas
  ${CMAKE_CURRENT_SOURCE_DIR}/src/canvas/items
  ${CMAKE_BINARY_DIR})

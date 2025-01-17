# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: CC0-1.0

cmake_minimum_required(VERSION 3.7)

project(dde-launchpad)

set(CMAKE_CXX_STANDARD 17) # blurhash requires 17, otherwish we can still use 14
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON) # ensure adapter class can include launchercontroller.h

set(BIN_NAME dde-launchpad)

include(FeatureSummary)
include(GNUInstallDirs)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Concurrent Qml Svg Quick QuickControls2 LinguistTools)
find_package(Dtk REQUIRED COMPONENTS Core Gui)
find_package(AppStreamQt REQUIRED)

set_package_properties(AppStreamQt PROPERTIES
    DESCRIPTION "Library that lists Appstream resources"
    URL "https://www.freedesktop.org"
    TYPE REQUIRED)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

add_subdirectory(systemd)
add_subdirectory(dbus)
add_subdirectory(src/gioutils)
add_subdirectory(src/utils)
add_subdirectory(src/quick)
add_subdirectory(src/ddeintegration)
add_subdirectory(src/models)

set(SOURCE_FILES
    main.cpp
    desktopintegration.cpp desktopintegration.h
    launchercontroller.cpp launchercontroller.h
    debughelper.cpp debughelper.h
)

set(QML_FILES
    qml/WindowedFrame.qml
    qml/FullscreenFrame.qml
    qml/AppItemMenu.qml
    qml/AppListView.qml
)

set(TRANSLATION_FILES
    translations/dde-launchpad.ts
    translations/dde-launchpad_zh_CN.ts
)

qt_create_translation(TRANSLATED_FILES ${QML_FILES} ${SOURCE_FILES} src/models/appitem.cpp ${TRANSLATION_FILES})

qt_add_dbus_adaptor(DBUS_ADAPTER_FILES dbus/org.deepin.dde.Launcher1.xml launchercontroller.h LauncherController)

add_executable(${BIN_NAME}
    ${SOURCE_FILES}
    ${DBUS_ADAPTER_FILES}
    qml.qrc
    ${TRANSLATED_FILES}
)

target_link_libraries(${BIN_NAME} PRIVATE
    Dtk::Core
    Dtk::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2

    gio-utils
    launcher-utils
    launcher-qml-utils
    launcher-models
    dde-integration-dbus
)

install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${TRANSLATED_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/${BIN_NAME}/translations)

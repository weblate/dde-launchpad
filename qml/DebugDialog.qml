// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

//import QtCore
import QtQml.Models 2.15
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import org.deepin.dtk 1.0
import org.kde.kitemmodels 1.0

import org.deepin.launchpad 1.0

Column {
    spacing: 15

    Switch {
        text: "Avoid launch application"
        checked: DebugHelper.avoidLaunchApp
        onCheckedChanged: {
            DebugHelper.avoidLaunchApp = checked
        }
    }

    Switch {
        text: "Avoid hide launchpad window"
        checked: DebugHelper.avoidHideWindow
        onCheckedChanged: {
            DebugHelper.avoidHideWindow = checked
        }
    }

    ToolButton {
        text: "Close launchpad application"
        onClicked: {
            Qt.quit()
        }
    }
}

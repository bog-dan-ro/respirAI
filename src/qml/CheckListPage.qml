/*
    Copyright (C) 2020, BogDan Vatra <bogdan@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.11
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

//@scope main.qml

Page {
    title: qsTr("Check list")

    ColumnLayout {
        anchors {
            fill: parent
            margins: 25
        }
        Item {
            Layout.fillHeight: true
        }
        Switch {
            id: firstCheck
            text: qsTr("First")
        }
        CheckBox {
            id: secondCheck
            text: qsTr("Second")
        }
        CheckBox {
            id: thirdCheck
            checked: true
            text: qsTr("Third")
        }
        Item {
            Layout.fillHeight: true
        }
        RowLayout {
            Layout.margins: 20
            DelayButton {
                text: qsTr("Back")
                onActivated: pageController.pop()
            }
            Item { Layout.fillWidth: true }
            DelayButton {
                text: qsTr("Next")
                enabled: firstCheck.checked && secondCheck.checked && thirdCheck.checked
                onActivated: pageController.push("ControlPage.qml")
            }
        }
    }
}

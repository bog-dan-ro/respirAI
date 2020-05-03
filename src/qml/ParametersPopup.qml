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

Popup {
    id: dialog
    closePolicy: Popup.NoAutoClose
    background: Rectangle {
        color: Qt.rgba(0, 0.5, 0.5, 0.75)
    }
    ColumnLayout {
        GridLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            columns: 4
            Label {
                textFormat: Text.RichText
                text: qsTr("FiO<sub>2</sub>")
            }
            SpinBox {
                editable: true
                from: 0
                to: 1000
                value: 500
            }
            Label {
                textFormat: Text.RichText
                text: qsTr("VT")
            }
            SpinBox {
                editable: true
                from: 0
                to: 1000
                value: 500
            }
            Label {
                textFormat: Text.RichText
                text: qsTr("T<sub>1</sub>")
            }
            SpinBox {
                editable: true
                from: 0
                to: 1000
                value: 500
            }
            Label {
                textFormat: Text.RichText
                text: qsTr("RR")
            }
            SpinBox {
                editable: true
                from: 0
                to: 1000
                value: 500
            }
            Label {
                textFormat: Text.RichText
                text: qsTr("PEEP")
            }
            SpinBox {
                editable: true
                from: 0
                to: 1000
                value: 500
            }
            Label {
                textFormat: Text.RichText
                text: qsTr("\u0394P<sub>supp</sub>")
            }
            SpinBox {
                editable: true
                from: 0
                to: 1000
                value: 500
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Button {
                text: qsTr("Cancel")
                onClicked: close()
            }
            Item { Layout.fillWidth: true }
            Button {
                text: qsTr("Ok")
                onClicked: close()
            }
        }
    }
}

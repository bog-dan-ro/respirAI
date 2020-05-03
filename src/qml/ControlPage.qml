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
import QtCharts 2.2

//@scope main.qml

Page {
    title: qsTr("Control Page")
    StackView.onActivated: window.header.visible = false
    StackView.onDeactivated: window.header.visible = true

    Connections {
        target: __sensorsDataManager
        onDataChanged: {
            pressure.setData(x, data.pressure)
            flow.setData(x, data.flow)
            volume.setData(x, data.volume)
        }
    }

    Component.onCompleted: {
        __sensorsDataManager.refreshHz = 30
        __sensorsDataManager.active = true
    }

    ParametersPopup {
        id: parametersPopup
        x: (parent.width - width) / 2
    }

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            id: chartsLayout
            spacing: -20 // remove the space between the charts
            Layout.fillHeight: true
            Layout.fillWidth: true

            LinesChart {
                id: pressure
                Layout.fillHeight: true
                Layout.fillWidth: true
                title: qsTr("Pressure (mbar)")
                valueYAxis {
                    min: -5
                    max: 60
                    tickCount: 5
                    labelFormat: "%.0f"
                }
            }

            LinesChart {
                id: flow
                Layout.fillHeight: true
                Layout.fillWidth: true
                title: qsTr("Air flow (L/min)")
                valueYAxis {
                    min: -50
                    max: 50
                    tickCount: 3
                    labelFormat: "%.0f"
                }
            }

            LinesChart {
                id: volume
                Layout.fillHeight: true
                Layout.fillWidth: true
                title: qsTr("Volume (mL)")
                valueYAxis {
                    min: 0
                    max: 500
                    tickCount: 2
                }
                valueXAxis.labelsVisible: true
            }
        }

        ColumnLayout {
            id: controlsLayout
            Layout.rightMargin: 5
            Layout.fillHeight: true
            Layout.fillWidth: false

            ValueLabel {
                Layout.fillWidth: true
                label: qsTr("Mode")
                value: "VC-SIMV"
            }

            ValueLabel {
                Layout.fillWidth: true
                label: qsTr("<b>VT</b> mL")
                value: "410"
                alarm: true
            }

            ValueLabel {
                Layout.fillWidth: true
                label: qsTr("<b>MV</b> L/min")
                value: "5.92"
            }

            ValueLabel {
                Layout.fillWidth: true
                label: qsTr("<b>PIP</b> mbar")
                value: "25"
            }

            ValueLabel {
                Layout.fillWidth: true
                label: qsTr("<b>R</b> mbar/L/s")
                value: "10"
            }

            ValueLabel {
                Layout.fillWidth: true
                label: qsTr("<b>C</b> mL/mbar")
                value: "34"
            }

            Button {
                text: qsTr("Parameters")
                icon.source: "configure.svg"
                onClicked: parametersPopup.open()
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}

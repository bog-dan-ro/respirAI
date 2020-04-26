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
import QtQuick.Window 2.11
import QtCharts 2.2

ChartView {
    id: chart
    theme: ChartView.ChartThemeDark
    dropShadowEnabled: true
    animationDuration: 0
    animationOptions: ChartView.NoAnimation
    antialiasing: true
    margins { top: 0; bottom: 0; left: 0; right: 0 }
    legend.visible: false
    property bool useOpenGL: true

    property alias seconds: valueXAxis.max
    property alias valueXAxis: valueXAxis
    property alias valueYAxis: valueYAxis

    property var first: leftSeries
    property var second: rightSeries

    property int pos: 0
    property int count: __sensorsDataManager.count
    property int blank: 5
    onCountChanged: resetData()
    onBlankChanged: resetData()

    function resetData()
    {
        first.clear()
        second.clear()
        if (!count)
            return;
        var ratio = seconds / count
        var x = blank * ratio;
        for (var i = blank; i < count;++i) {
            second.append(x, 0);
            x += ratio;
        }
        pos = 0;
    }

    function setData(x, y)
    {
        // are we at end? Swap the lines
        if (++pos >= count) {
            var tmp = first;
            first = second;
            second = tmp;
            pos = 0;
        }
        first.append(x, y);
        if (second.count)
            second.remove(0);
        else
            first.remove(0);
    }

    // Define x-axis to be used with the series instead of default one
    ValueAxis {
        id: valueXAxis
        min: 0
        max: 15
        tickCount: 15
        labelFormat: "%.0f"
        labelsVisible: false
    }
    ValueAxis {
        id: valueYAxis
    }

    LineSeries {
        id: leftSeries
        axisX: valueXAxis
        axisY: valueYAxis
        width: 1
        useOpenGL: chart.useOpenGL
    }

    LineSeries {
        id: rightSeries
        axisX: valueXAxis
        axisY: valueYAxis
        width: 1
        useOpenGL: chart.useOpenGL
    }
}

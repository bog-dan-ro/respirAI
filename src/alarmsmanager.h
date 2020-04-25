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

#pragma once

#include <QObject>

class AlarmsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentAlarm READ currentAlarm NOTIFY currentAlarmChanged)
public:
    explicit AlarmsManager(QObject *parent = nullptr);
    QString currentAlarm() const;
    void setCurrentAlarm(const QString &currentAlarm= {});

public slots:
    void acknowledgeAlarm();

signals:
    void currentAlarmChanged(QString currentAlarm);

private:
    QString m_currentAlarm;
};

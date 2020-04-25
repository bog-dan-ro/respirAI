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

#include "alarmsmanager.h"

AlarmsManager::AlarmsManager(QObject *parent) : QObject(parent)
{}

QString AlarmsManager::currentAlarm() const
{
    return m_currentAlarm;
}

void AlarmsManager::setCurrentAlarm(const QString &currentAlarm)
{
    if (m_currentAlarm == currentAlarm)
        return;
    m_currentAlarm = currentAlarm;
    emit currentAlarmChanged(m_currentAlarm);
}

void AlarmsManager::acknowledgeAlarm()
{
    setCurrentAlarm();
}

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

#include "sensorsdatamanager.h"

SensorsDataManager::SensorsDataManager(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<SensorsData>();
    connect(&m_refreshTimer, &QTimer::timeout, this, [this]{
        SensorsData data;
        {
            QMutexLocker lock(&m_dataMutex);
            data = m_cachedData;
            if (++m_currentPos >=  m_count)
                m_currentPos = 0;
        }
        emit dataChanged(double(m_currentPos) / m_refreshHz, data);
    });
    setRefreshHz(10);
}

bool SensorsDataManager::active() const
{
    return m_refreshTimer.isActive();
}

void SensorsDataManager::setActive(bool active)
{
    if (m_refreshTimer.isActive() == active)
        return;
    if (active)
        m_refreshTimer.start();
    else
        m_refreshTimer.stop();
    resetData();
    emit activeChanged(m_refreshTimer.isActive());
}

int SensorsDataManager::refreshHz() const
{
    return m_refreshHz;
}

void SensorsDataManager::setRefreshHz(int refreshHz)
{
    if (refreshHz < 1)
        return;

    if (m_refreshHz == refreshHz)
        return;

    // No need to restart the timer as setInterval takes care of it
    m_refreshHz = refreshHz;
    m_refreshTimer.setInterval(1000 / m_refreshHz);
    resetData();
    emit refreshHzChanged(m_refreshHz);
}

int SensorsDataManager::secondsCache() const
{
    return m_secondsCache;
}

void SensorsDataManager::setSecondsCache(int secondsCache)
{
    if (m_secondsCache == secondsCache)
        return;

    m_secondsCache = secondsCache;
    resetData();
    emit secondsCacheChanged(m_secondsCache);
}

int SensorsDataManager::count() const
{
    return m_count;
}

void SensorsDataManager::setData(const SensorsData &data)
{
    QMutexLocker lock(&m_dataMutex);
    m_cachedData = data;
}

void SensorsDataManager::resetData()
{
    m_currentPos = 0;
    m_count = m_secondsCache * m_refreshHz;
    emit countChanged();
}

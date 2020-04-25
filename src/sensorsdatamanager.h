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

#include <QMutex>
#include <QObject>
#include <QTimer>

struct SensorsData
{
    Q_GADGET
    Q_PROPERTY(double pressure MEMBER pressure CONSTANT)
    Q_PROPERTY(double flow MEMBER flow CONSTANT)
    Q_PROPERTY(double volume MEMBER volume CONSTANT)

public:
    double pressure = .0;
    double flow = .0;
    double volume = .0;
};

class SensorsDataManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(int refreshHz READ refreshHz WRITE setRefreshHz NOTIFY refreshHzChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int secondsCache READ secondsCache WRITE setSecondsCache NOTIFY secondsCacheChanged)

public:
    explicit SensorsDataManager(QObject *parent = nullptr);

    bool active() const;
    void setActive(bool active);
    int refreshHz() const;
    void setRefreshHz(int refreshHz);
    int secondsCache() const;
    void setSecondsCache(int secondsCache);
    int count() const;

public slots:
    void setData(const SensorsData &data);

signals:
    void activeChanged(bool active);
    void refreshHzChanged(int refreshHz);
    void secondsCacheChanged(int secondsCache);
    void countChanged();
    void dataChanged(double x, const SensorsData &data);

private:
    void resetData();

private:
    int m_currentPos = 0;
    SensorsData m_cachedData;
    mutable QMutex m_dataMutex;
    int m_refreshHz = -1;
    int m_secondsCache = 15;
    int m_count;
    double m_factor = 1;
    QTimer m_refreshTimer;
};

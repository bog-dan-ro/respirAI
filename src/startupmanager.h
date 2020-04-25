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

class StartupManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

public:
    enum class Status {
        Checked,
        Running,
        Failure
    };
    Q_ENUM(Status)

public:
    explicit StartupManager(QObject *parent = nullptr);
    Status status() const;
    void setStatus(Status status);
public slots:
    void retry();

signals:
    void statusChanged(Status status);

private:
    Status m_status = Status::Running;
};

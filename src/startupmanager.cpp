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

#include "startupmanager.h"
#ifdef DUMMY_DATA
# include <QTimer>
#endif

StartupManager::StartupManager(QObject *parent)
    : QObject(parent)
{}

StartupManager::Status StartupManager::status() const
{
    return m_status;
}

void StartupManager::setStatus(Status status)
{
    if (m_status == status)
        return;
    m_status = status;
    emit statusChanged(status);
}

void StartupManager::retry()
{
    setStatus(Status::Running);
#ifdef DUMMY_DATA
    QTimer::singleShot(2500 + qrand() % 2500, this, [this] {
        setStatus(qrand() % 2 ? Status::Checked : Status::Failure);
    });
#else
    #warning IMPLEMNT_ME
    setStatus(Status::Checked);
#endif
}

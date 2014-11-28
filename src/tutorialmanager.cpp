/*
Copyright (C) 2012 Sebastian Herbord. All rights reserved.

This file is part of Mod Organizer.

Mod Organizer is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Mod Organizer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Mod Organizer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tutorialmanager.h"
#include "tutorialcontrol.h"
#include "utility.h"
#include <QDir>
#include <QString>
#include <QDebug>

namespace MOBase {


TutorialManager *TutorialManager::s_Instance = nullptr;


TutorialManager::TutorialManager(const QString &tutorialPath)
  : m_TutorialPath(tutorialPath)
{
  // m_TutorialPath(QDir::fromNativeSeparators(ToQString(GameInfo::instance().getTutorialDir())).append("/"))
}


void TutorialManager::init(const QString &tutorialPath)
{
  if (s_Instance == nullptr) {
    s_Instance = new TutorialManager(tutorialPath);
  } else {
    throw MyException(tr("tutorial manager already initialized"));
  }
}


TutorialManager &TutorialManager::instance()
{
  if (s_Instance == nullptr) {
    throw MyException(tr("tutorial manager not set up yet"));
  }
  return *s_Instance;
}


void TutorialManager::activateTutorial(const QString &windowName, const QString &tutorialName)
{
  std::map<QString, TutorialControl*>::iterator iter = m_Controls.find(windowName);
  if (iter != m_Controls.end()) {
    // control already visible, start tutorial right away
    iter->second->startTutorial(m_TutorialPath + tutorialName);
  } else {
    m_PendingTutorials[windowName] = tutorialName;
  }
}


void TutorialManager::finishWindowTutorial(const QString &windowName)
{
  emit windowTutorialFinished(windowName);
//  QSettings &settings = Settings::instance().directInterface();
//  settings.setValue(QString("CompletedWindowTutorials/") + windowName, true);
}


bool TutorialManager::hasTutorial(const QString &tutorialName)
{
  return QFile::exists(m_TutorialPath + tutorialName);
}


void TutorialManager::registerControl(const QString &windowName, TutorialControl *control)
{
  m_Controls[windowName] = control;
  std::map<QString, QString>::iterator iter = m_PendingTutorials.find(windowName);
  if (iter != m_PendingTutorials.end()) {
    // there is a pending tutorial for this window, display it
    control->startTutorial(m_TutorialPath + iter->second);
    m_PendingTutorials.erase(iter);
  }
}


void TutorialManager::unregisterControl(const QString &windowName)
{
  std::map<QString, TutorialControl*>::iterator iter = m_Controls.find(windowName);
  if (iter != m_Controls.end()) {
    m_Controls.erase(iter);
  } else {
    qWarning() << "failed to remove tutorial control " << windowName;
  }
}
} // namespace MOBase

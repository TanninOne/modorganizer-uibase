#include "executableinfo.h"

using namespace MOBase;

QString ExecutableInfo::title() const
{
  return m_Title;
}

QFileInfo ExecutableInfo::binary() const
{
  return m_Binary;
}


QStringList ExecutableInfo::arguments() const
{
  return m_Arguments;
}

QDir ExecutableInfo::workingDirectory() const
{
  return m_WorkingDirectory;
}

ExecutableInfo::CloseMOStyle ExecutableInfo::closeMO() const
{
  return m_CloseMO;
}

QString ExecutableInfo::steamAppID() const
{
  return m_SteamAppID;
}

bool ExecutableInfo::isCustom() const
{
  return m_Custom;
}

bool ExecutableInfo::showInToolbar() const
{
  return m_ShowInToolbar;
}


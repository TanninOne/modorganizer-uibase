#ifndef EXECUTABLEINFO_H
#define EXECUTABLEINFO_H


#include <QString>
#include <QFileInfo>
#include <QDir>


namespace MOBase {

class ExecutableInfo
{
public:

  enum class CloseMOStyle {
    DEFAULT_CLOSE = 1,
    DEFAULT_STAY = 2,
    NEVER_CLOSE = 3
  };

public:

  ExecutableInfo(const QString &title, const QFileInfo &binary)
    : m_Title(title)
    , m_Binary(binary)
    , m_WorkingDirectory(binary.absoluteDir())
    , m_CloseMO(CloseMOStyle::DEFAULT_STAY)
    , m_SteamAppID()
  {
  }

  ExecutableInfo &withArgument(const QString &argument)
  {
    m_Arguments.append(argument);
    return *this;
  }

  ExecutableInfo &withWorkingDirectory(const QDir &workingDirectory)
  {
    m_WorkingDirectory = workingDirectory;
    return *this;
  }

  ExecutableInfo &withSteamAppId(const QString &appId)
  {
    m_SteamAppID = appId;
    return *this;
  }

  ExecutableInfo &withDefaultClose()
  {
    m_CloseMO = CloseMOStyle::DEFAULT_CLOSE;
    return *this;
  }

  ExecutableInfo &withNeverClose()
  {
    m_CloseMO = CloseMOStyle::NEVER_CLOSE;
    return *this;
  }

  ExecutableInfo &asCustom()
  {
    m_Custom = true;
    return *this;
  }

  void showInToolbar(bool show)
  {
    m_ShowInToolbar = show;
  }

  QString title() const;
  QFileInfo binary() const;
  QStringList arguments() const;
  QDir workingDirectory() const;
  CloseMOStyle closeMO() const;
  QString steamAppID() const;
  bool isCustom() const;
  bool showInToolbar() const;

private:

  QString m_Title;
  QFileInfo m_Binary;
  QStringList m_Arguments;
  QDir m_WorkingDirectory;
  CloseMOStyle m_CloseMO { CloseMOStyle::DEFAULT_STAY };
  QString m_SteamAppID;
  bool m_Custom { false };
  bool m_ShowInToolbar { false };

};

} // namespace MOBase

#endif // EXECUTABLEINFO_H

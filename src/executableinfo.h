#ifndef EXECUTABLEINFO_H
#define EXECUTABLEINFO_H


#include "dllimport.h"
#include <QString>
#include <QFileInfo>
#include <QDir>


namespace MOBase {

class QDLLEXPORT ExecutableInfo
{
public:

  enum class CloseMOStyle {
    DEFAULT_CLOSE = 1,
    DEFAULT_STAY = 2,
    NEVER_CLOSE = 3
  };

public:

  ExecutableInfo(const QString &title, const QFileInfo &binary);

  ExecutableInfo &withArgument(const QString &argument);

  ExecutableInfo &withWorkingDirectory(const QDir &workingDirectory);

  ExecutableInfo &withSteamAppId(const QString &appId);

  ExecutableInfo &withDefaultClose();

  ExecutableInfo &withNeverClose();

  ExecutableInfo &asCustom();

  void showInToolbar(bool show);

  bool isValid() const;

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

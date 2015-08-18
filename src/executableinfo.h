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

  ExecutableInfo(const QString &title, const QFileInfo &binary);

  ExecutableInfo &withArgument(const QString &argument);

  ExecutableInfo &withWorkingDirectory(const QDir &workingDirectory);

  ExecutableInfo &withSteamAppId(const QString &appId);

  ExecutableInfo &withDefaultClose();

  ExecutableInfo &withNeverClose();

  ExecutableInfo &withAlwaysClose();

  ExecutableInfo &asCustom();

  bool isValid() const;

  QString title() const;
  QFileInfo binary() const;
  QStringList arguments() const;
  QDir workingDirectory() const;
  QString steamAppID() const;
  bool isCustom() const;
  bool closeByDefault() const;
  bool disableCloseSelection() const;

private:

  QString m_Title;
  QFileInfo m_Binary;
  QStringList m_Arguments;
  QDir m_WorkingDirectory;
  QString m_SteamAppID;
  bool m_Custom { false };
  bool m_CloseByDefault { false };
  bool m_DisableCloseSelection { false };

};

} // namespace MOBase

#endif // EXECUTABLEINFO_H

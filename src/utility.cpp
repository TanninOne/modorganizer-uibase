/*
Mod Organizer shared UI functionality

Copyright (C) 2012 Sebastian Herbord. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "utility.h"
#include "report.h"
#include <QDir>
#include <QBuffer>
#include <QDesktopWidget>
#include <QApplication>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ShlObj.h>

namespace MOBase {


MyException::MyException(const QString &text)
  : std::exception(), m_Message(text.toLocal8Bit())
{
}


bool removeDir(const QString &dirName)
{
  QDir dir(dirName);

  if (dir.exists()) {
    Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
      if (info.isDir()) {
        if (!removeDir(info.absoluteFilePath())) {
          return false;
        }
      } else {
        ::SetFileAttributesW(ToWString(info.absoluteFilePath()).c_str(), FILE_ATTRIBUTE_NORMAL);
        QFile file(info.absoluteFilePath());
        if (!file.remove()) {
          reportError(QObject::tr("removal of \"%1\" failed: %2").arg(info.absoluteFilePath()).arg(file.errorString()));
          return false;
        }
      }
    }

    if (!dir.rmdir(".")) {
      reportError(QObject::tr("removal of \"%1\" failed").arg(dir.absolutePath()));
      return false;
    }
  } else {
    reportError(QObject::tr("\"%1\" doesn't exist (remove)").arg(dirName));
    return false;
  }

  return true;
}


bool copyDir(const QString &sourceName, const QString &destinationName, bool merge)
{
  QDir sourceDir(sourceName);
  if (!sourceDir.exists()) {
    return false;
  }
  QDir destDir(destinationName);
  if (!destDir.exists()) {
    destDir.mkdir(destinationName);
  } else if (!merge) {
    return false;
  }

  QStringList files = sourceDir.entryList(QDir::Files);
  foreach (QString fileName, files) {
    QString srcName = sourceName + "/" + fileName;
    QString destName = destinationName + "/" + fileName;
    QFile::copy(srcName, destName);
  }

  files.clear();
  // we leave out symlinks because that could cause an endless recursion
  QStringList subDirs = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
  foreach (QString subDir, subDirs) {
    QString srcName = sourceName + "/" + subDir;
    QString destName = destinationName + "/" + subDir;
    copyDir(srcName, destName, merge);
  }
  return true;
}

bool moveFileRecursive(const QString &source, const QString &baseDir, const QString &destination)
{
  QStringList pathComponents = destination.split("/");
  QString path = baseDir;
  for (QStringList::Iterator iter = pathComponents.begin(); iter != pathComponents.end() - 1; ++iter) {
    path.append("/").append(*iter);
    if (!QDir(path).exists() && !QDir().mkdir(path)) {
      reportError(QObject::tr("failed to create directory \"%1\"").arg(path));
      return false;
    }
  }

  QString destinationAbsolute = baseDir.mid(0).append("/").append(destination);
  if (!QFile::rename(source, destinationAbsolute)) {
    // move failed, try copy & delete
    if (!QFile::copy(source, destinationAbsolute)) {
      reportError(QObject::tr("failed to copy \"%1\" to \"%2\"").arg(source).arg(destinationAbsolute));
      return false;
    } else {
      QFile::remove(source);
    }
  }
  return true;
}

bool copyFileRecursive(const QString &source, const QString &baseDir, const QString &destination)
{
  QStringList pathComponents = destination.split("/");
  QString path = baseDir;
  for (QStringList::Iterator iter = pathComponents.begin(); iter != pathComponents.end() - 1; ++iter) {
    path.append("/").append(*iter);
    if (!QDir(path).exists() && !QDir().mkdir(path)) {
      reportError(QObject::tr("failed to create directory \"%1\"").arg(path));
      return false;
    }
  }

  QString destinationAbsolute = baseDir.mid(0).append("/").append(destination);
  if (!QFile::copy(source, destinationAbsolute)) {
    reportError(QObject::tr("failed to copy \"%1\" to \"%2\"").arg(source).arg(destinationAbsolute));
    return false;
  }
  return true;
}


std::wstring ToWString(const QString &source)
{
  wchar_t *buffer = new wchar_t[source.count() + 1];
  source.toWCharArray(buffer);
  buffer[source.count()] = L'\0';
  std::wstring result(buffer);
  delete [] buffer;

  return result;
}

QString ToQString(const std::wstring &source)
{
  return QString::fromUtf16(source.c_str());
}


QString ToString(const SYSTEMTIME &time)
{
  char dateBuffer[100];
  char timeBuffer[100];
  int size = 100;
  GetDateFormatA(LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &time, NULL, dateBuffer, size);
  GetTimeFormatA(LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &time, NULL, timeBuffer, size);
  return QString::fromLocal8Bit(dateBuffer) + " " + QString::fromLocal8Bit(timeBuffer);
}

bool fixDirectoryName(QString &name)
{
  QString temp = name.simplified();
  while (temp.endsWith('.')) temp.chop(1);

  temp.replace(QRegExp("[<>:\"/\\|?*]"), "");
  static QString invalidNames[] = { "CON", "PRN", "AUX", "NUL", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
                                    "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9" };
  for (int i = 0; i < sizeof(invalidNames) / sizeof(QString); ++i) {
    if (temp == invalidNames[i]) {
      temp = "";
      break;
    }
  }

  if (temp.length() > 1) {
    name = temp;
    return true;
  } else {
    return false;
  }
}

QString getDesktopDirectory()
{
  wchar_t desktop[32768];
  SHGetSpecialFolderPathW(NULL, desktop, CSIDL_DESKTOPDIRECTORY, 0);
  return QString::fromUtf16(desktop);
}

QString getStartMenuDirectory()
{
  wchar_t desktop[32768];
  SHGetSpecialFolderPathW(NULL, desktop, CSIDL_STARTMENU, 0);
  return QString::fromUtf16(desktop);
}
} // namespace MOBase

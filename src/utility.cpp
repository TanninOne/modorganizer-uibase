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
#include <boost/scoped_array.hpp>
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


QString windowsErrorString(DWORD errorCode)
{
  QByteArray result;
  QTextStream stream(&result);

  LPWSTR buffer = NULL;
  // TODO: the message is not english?
  if (FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                     NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buffer, 0, NULL) == 0) {
    stream << " (errorcode " << errorCode << ")";
  } else {
    // remove line break
    LPWSTR lastChar = buffer + wcslen(buffer) - 2;
    *lastChar = L'\0';
    stream << ToQString(buffer) << " (errorcode " << errorCode << ")";
    LocalFree(buffer); // allocated by FormatMessage
  }
  stream.flush();
  return QString(result);
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


static DWORD TranslateError(int error)
{
  switch (error) {
    case 0x71:    return ERROR_INVALID_PARAMETER; // same file
    case 0x72:    return ERROR_INVALID_PARAMETER; // many source, one destination. shouldn't happen due to how parameters are transformed
    case 0x73:    return ERROR_NOT_SAME_DEVICE;
    case 0x74:    return ERROR_INVALID_PARAMETER;
    case 0x75:    return ERROR_CANCELLED;
    case 0x76:    return ERROR_BAD_PATHNAME;
    case 0x78:    return ERROR_ACCESS_DENIED;
    case 0x79:    return ERROR_BUFFER_OVERFLOW; // path exceeds max_path
    case 0x7A:    return ERROR_INVALID_PARAMETER;
    case 0x7C:    return ERROR_BAD_PATHNAME;
    case 0x7D:    return ERROR_INVALID_PARAMETER;
    case 0x7E:    return ERROR_ALREADY_EXISTS;
    case 0x80:    return ERROR_ALREADY_EXISTS;
    case 0x81:    return ERROR_BUFFER_OVERFLOW;
    case 0x82:    return ERROR_WRITE_PROTECT;
    case 0x83:    return ERROR_WRITE_PROTECT;
    case 0x84:    return ERROR_WRITE_PROTECT;
    case 0x85:    return ERROR_DISK_FULL;
    case 0x86:    return ERROR_WRITE_PROTECT;
    case 0x87:    return ERROR_WRITE_PROTECT;
    case 0x88:    return ERROR_WRITE_PROTECT;
    case 0xB7:    return ERROR_BUFFER_OVERFLOW;
    case 0x402:   return ERROR_PATH_NOT_FOUND;
    case 0x10000: return ERROR_GEN_FAILURE;
    default: return static_cast<DWORD>(error);
  }
}


static bool shellOp(const QStringList &sourceNames, const QStringList &destinationNames, QWidget *dialog, UINT operation)
{
  std::vector<wchar_t> fromBuffer;
  std::vector<wchar_t> toBuffer;

  foreach (const QString &from, sourceNames) {
    // SHFileOperation has to be used with absolute maths, err paths ("It cannot be overstated" they say)
    std::wstring tempFrom = ToWString(QDir::toNativeSeparators(QFileInfo(from).absoluteFilePath()));
    fromBuffer.insert(fromBuffer.end(), tempFrom.begin(), tempFrom.end());
    fromBuffer.push_back(L'\0');
  }

  if ((destinationNames.count() == sourceNames.count()) ||
      (destinationNames.count() == 1)) {
    foreach (const QString &to, destinationNames) {
      std::wstring tempTo = ToWString(QDir::toNativeSeparators(QFileInfo(to).absoluteFilePath()));
      toBuffer.insert(toBuffer.end(), tempTo.begin(), tempTo.end());
      toBuffer.push_back(L'\0');
    }
  } else if ((operation == FO_DELETE) && (destinationNames.count() == 0)) {
    // pTo is not used but as I understand the documentation it should still be double-null terminated
    toBuffer.push_back(L'\0');
  } else {
    ::SetLastError(ERROR_INVALID_PARAMETER);
    return false;
  }

  // both buffers have to be double-null terminated
  fromBuffer.push_back(L'\0');
  toBuffer.push_back(L'\0');

  SHFILEOPSTRUCTW op;
  if (dialog != NULL) {
    op.hwnd = dialog->winId();
  } else {
    op.hwnd = NULL;
  }
  op.wFunc = operation;
  op.pFrom = &fromBuffer[0];
  op.pTo = &toBuffer[0];

  if (operation == FO_DELETE) {
    op.fFlags = FOF_NOCONFIRMATION;
  } else {
    op.fFlags = FOF_NOCOPYSECURITYATTRIBS |  // always use security of target directory
                FOF_SILENT |                 // don't show a progress bar
                FOF_NOCONFIRMMKDIR;          // silently create directories

    if (destinationNames.count() == sourceNames.count()) {
      op.fFlags |= FOF_MULTIDESTFILES;
    }
  }

  int res = ::SHFileOperationW(&op);
  if (res == 0) {
    return true;
  } else {
    ::SetLastError(TranslateError(res));
    return false;
  }
}

bool shellCopy(const QStringList &sourceNames, const QStringList &destinationNames, QWidget *dialog)
{
  return shellOp(sourceNames, destinationNames, dialog, FO_COPY);
}

bool shellMove(const QStringList &sourceNames, const QStringList &destinationNames, QWidget *dialog)
{
  return shellOp(sourceNames, destinationNames, dialog, FO_MOVE);
}

bool shellRename(const QString &oldName, const QString &newName, QWidget *dialog)
{
  return shellOp(QStringList(oldName), QStringList(newName), dialog, FO_RENAME);
}

bool shellDelete(const QStringList &fileNames, QWidget *dialog)
{
  return shellOp(fileNames, QStringList(), dialog, FO_DELETE);
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

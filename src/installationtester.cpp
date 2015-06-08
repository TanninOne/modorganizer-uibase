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

#include "installationtester.h"
#include "filenamestring.h"

#include <QFileInfo>
#include <set>

namespace MOBase {


InstallationTester::InstallationTester()
{
}


bool InstallationTester::isTopLevelDirectory(const FileNameString &dirName)
{
  static QString tlDirectoryNames[] = { "distantlod", "facegen", "fonts", "interface", "menus", "meshes", "music", "scripts", "shaders", "sound",
                                        "strings", "textures", "trees", "video", "skse", "obse", "nvse", "fose", "asi", "SkyProc Patchers", "" };

  for (int i = 0; tlDirectoryNames[i].length() != 0; ++i) {
    if (dirName == tlDirectoryNames[i]) {
      return true;
    }
  }

  return false;
}


bool InstallationTester::isTopLevelDirectoryBain(const FileNameString &dirName)
{
  static QString tlDirectoryNames[] = { "distantlod", "facegen", "fonts", "interface", "menus", "meshes", "music", "scripts", "shaders", "sound",
                                        "strings", "textures", "trees", "video", "skse", "obse", "nvse", "fose", "asi", "SkyProc Patchers", "Docs", "INI Tweaks", "" };

  for (int i = 0; tlDirectoryNames[i].length() != 0; ++i) {
    if (dirName == tlDirectoryNames[i]) {
      return true;
    }
  }

  return false;
}


bool InstallationTester::isTopLevelSuffix(const FileNameString &fileName)
{
  static std::set<QString> tlSuffixes { "esp", "esm", "bsa" };

  return tlSuffixes.find(QFileInfo(static_cast<QString>(fileName)).suffix()) != tlSuffixes.end();
}

/*
bool InstallationTester::isSuffixAcceptable(const QString &fileName) {
  static QString acceptableSuffixes[] = { "esp", "esm", "bsa", "txt", "jpg", "png", "" };

  QString suffix = QFileInfo(fileName).suffix();

  for (int i = 0; acceptableSuffixes[i].length() != 0; ++i) {
    if (QString::compare(suffix, acceptableSuffixes[i], Qt::CaseInsensitive) == 0) {
      return true;
    }
  }

  return false;
}
*/
} // namespace MOBase

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


#ifndef IMOINFO_H
#define IMOINFO_H


#include "versioninfo.h"
#include "guessedvalue.h"
#include "imodinterface.h"
#include "igameinfo.h"
#include "imodrepositorybridge.h"
#include "idownloadmanager.h"
#include <QString>
#include <QVariant>

namespace MOBase {

/**
 * @brief Interface to class that provides information about the running session
 *        of Mod Organizer to be used by plugins
 */
class IOrganizer {

public:

  virtual ~IOrganizer() {}

  /**
   * @return information structure holding information about the game being managed
   */
  virtual IGameInfo &gameInfo() const = 0;

  /**
   * @return create a new nexus interface class
   */
  virtual IModRepositoryBridge *createNexusBridge() const = 0;

  /**
   * @return name of the active profile or an empty string if no profile is loaded (yet)
   */
  virtual QString profileName() const = 0;

  /**
   * @return the (absolute) path to the active profile or an empty string if no profile is loaded (yet)
   */
  virtual QString profilePath() const = 0;

  /**
   * @return the (absolute) path to the download directory
   */
  virtual QString downloadsPath() const = 0;

  /**
   * @return the running version of Mod Organizer
   */
  virtual VersionInfo appVersion() const = 0;

  /**
   * @brief retrieve an interface to a mod by its name
   * @param name name of the mod to query
   * @return an interface to the mod or NULL if there is no mod with the name
   */
  virtual IModInterface *getMod(const QString &name) = 0;

  /**
   * @brief create a new mod with the specified name
   * @param name name of the new mod
   * @return an interface that can be used to modify the mod. NULL if the user canceled
   * @note an exception is thrown if the mod already exists. Use "getMod" to verify
   *       the mod-name is unused first
   */
  virtual IModInterface *createMod(GuessedValue<QString> &name) = 0;

  /**
   * @brief remove a mod (from disc and from the ui)
   * @param mod the mod to remove
   * @return true on success, false on error
   */
  virtual bool removeMod(IModInterface *mod) = 0;

  /**
   * @brief let the organizer know that a mod has changed
   * @param the mod that has changed
   */
  virtual void modDataChanged(IModInterface *mod) = 0;

  /**
   * @brief retrieve the specified setting for a plugin
   * @param pluginName name of the plugin for which to retrieve a setting
   * @param key identifier of the setting
   * @return the setting
   * @throw an exception is thrown if the specified setting hasn't been declared
   */
  virtual QVariant pluginSetting(const QString &pluginName, const QString &key) const = 0;

  /**
   * @return path to a directory where plugin data should be stored.
   */
  virtual QString pluginDataPath() const = 0;

  /**
   * @brief install a mod archive at the specified location
   * @param fileName absolute file name of the mod to install
   */
  virtual void installMod(const QString &fileName) = 0;

  /**
   * @brief resolves a path relative to the virtual data directory to its absolute real path
   * @param fileName path to resolve
   * @return the absolute real path or an empty string
   */
  virtual QString resolvePath(const QString &fileName) const = 0;

  /**
   * @return interface to the download manager
   */
  virtual MOBase::IDownloadManager *downloadManager() = 0;

};

} // namespace MOBase

#endif // IMOINFO_H

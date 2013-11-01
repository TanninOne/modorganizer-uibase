/*
Mod Organizer shared UI functionality

Copyright (C) 2013 Sebastian Herbord. All rights reserved.

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


#ifndef IMODLIST_H
#define IMODLIST_H

#include <QString>

namespace MOBase {

class IModList {

public:

  enum ModState {
    STATE_MISSING,
    STATE_INACTIVE,
    STATE_ACTIVE,
    STATE_NOTOPTIONAL
  };

public:

  /**
   * @brief retrieve the state of a mod
   * @param name name of the mod
   * @return one of the possible mod states: missing, inactive or active
   */
  virtual ModState state(const QString &name) const = 0;

  /**
   * @brief retrieve the priority of a mod
   * @param name name of the mod
   * @return the priority (the higher the more important). Returns -1 if the mod doesn't exist
   */
  virtual int priority(const QString &name) const = 0;

  /**
   * @brief change the priority of a mod
   * @param name name of the mod
   * @param newPriority new priority of the mod
   * @return true on success, false if the priority change was not possible. This is usually because one of the parameters is invalid
   * @note Very important: newPriority is the new priority after the move. Keep in mind that the mod disappears from it's old location and
   *       all mods with higher priority than the moved mod decrease in priority by one.
   */
  virtual bool setPriority(const QString &name, int newPriority) = 0;

};

}

#endif // IMODLIST_H

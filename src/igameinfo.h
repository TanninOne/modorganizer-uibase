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


#ifndef IGAMEINFO_H
#define IGAMEINFO_H

#include <QString>

namespace MOBase {

class IGameInfo {

public:

  enum Type {
    TYPE_OBLIVION,
    TYPE_FALLOUT3,
    TYPE_FALLOUT4,
    TYPE_FALLOUTNV,
    TYPE_SKYRIM
  };

public:

  virtual ~IGameInfo() {}

  /**
   * @return the type of game being managed
   */
  virtual Type type() const = 0;

  /**
   * @return the (absolute) path to the game directory
   */
  virtual QString path() const = 0;

  /**
   * @return name of the game binary (i.e. TESV.exe)
   */
  virtual QString binaryName() const = 0;

  /**
   * @return version of game
   */
  virtual QString version() const = 0;

  /**
   * @return version of script extender or empty string
   */
  virtual QString extenderVersion() const = 0;
};


} // namespace MOBase

#endif // IGAMEINFO_H

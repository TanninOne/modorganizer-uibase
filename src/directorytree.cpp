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

#include "directorytree.h"

namespace MOBase {

bool operator<(const FileTreeInformation &LHS, const FileTreeInformation &RHS) {
  return LHS.m_Name < RHS.m_Name;
}


bool operator<(const DirectoryTreeInformation &LHS, const DirectoryTreeInformation &RHS)
{
  return LHS.name < RHS.name;
}


} // namespace MOBase

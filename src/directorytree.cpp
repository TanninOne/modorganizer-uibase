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

template <>
QString DirectoryTree::getFullPath(FileTreeInformation const *leaf) const
{
  //Warning: I have difficulty persuading myself that this'll return the correct
  //result under all circumstances, but so far it seems to be fine.
  //Intended return is:
  //"" for top level of tree
  //"a" for leaves or nodes at the top of the tree
  //"a\b"  for leaves or nodes at the 2nd level of the tree
  //etc
  QString result;
  if (leaf != nullptr) {
    result = leaf->getName().toQString();
  }
  const Node *parent = this;
  while (parent != nullptr) {
    if (parent->getParent() != nullptr) {
      result.prepend("\\");
    }
    result.prepend(parent->getData().name.toQString());
    parent = parent->getParent();
  }
  return result;
}


} // namespace MOBase

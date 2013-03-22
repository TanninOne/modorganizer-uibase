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

#ifndef DIRECTORYTREE_H
#define DIRECTORYTREE_H

#include "mytree.h"
#include "dllimport.h"
#include <QMetaType>

namespace MOBase {


class FileTreeInformation {
  friend QDLLEXPORT bool operator<(const FileTreeInformation &LHS, const FileTreeInformation &RHS);
public:
  FileTreeInformation() : m_Name(), m_Index(0) {}
  FileTreeInformation(const FileTreeInformation &reference) : m_Name(reference.m_Name), m_Index(reference.m_Index) {}
  FileTreeInformation(const QString &name, int index) : m_Name(name), m_Index(index) {}
  const QString &getName() const { return m_Name; }
  int getIndex() const { return m_Index; }
private:
  QString m_Name;
  int m_Index;
};

Q_DECLARE_METATYPE(FileTreeInformation)


struct DirectoryTreeInformation {
  DirectoryTreeInformation() : name(), index(-1) { }
  DirectoryTreeInformation(const DirectoryTreeInformation &reference) : name(reference.name), index(reference.index) { }
  DirectoryTreeInformation(const QString &name) : name(name), index(-1) { }
  DirectoryTreeInformation(const QString &name, int index) : name(name), index(index) { }
  DirectoryTreeInformation &operator=(const DirectoryTreeInformation &reference) {
    if (this != &reference) {
      name = reference.name;
      index = reference.index;
    }
    return *this;
  }

  QString name;
  int index;
};

QDLLEXPORT bool operator<(const DirectoryTreeInformation &LHS, const DirectoryTreeInformation &RHS);


/**
 * A tree representing the content of a directory structures with subdirectories as the nodes
 * and files as the leafs
 */
typedef MyTree<FileTreeInformation, DirectoryTreeInformation> DirectoryTree;

} // namespace MOBase

#endif // DIRECTORYTREE_H

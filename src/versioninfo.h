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

#ifndef MODVERSION_H
#define MODVERSION_H


#include "dllimport.h"
#include <QString>


/**
 * @brief represents the version of a mod or plugin
 *
 * this will try to parse machine-readable information from a string.
 **/
class QDLLEXPORT VersionInfo
{

  friend QDLLEXPORT bool operator<(const VersionInfo &LHS, const VersionInfo &RHS);
  friend QDLLEXPORT bool operator<=(const VersionInfo &LHS, const VersionInfo &RHS);

public:

  enum ReleaseType {
    RELEASE_PREALPHA,
    RELEASE_ALPHA,
    RELEASE_BETA,
    RELEASE_CANDIDATE,
    RELEASE_FINAL
  };

public:

  /**
   * @brief default constructor
   * constructs an invalid version
   **/
  VersionInfo();

  /**
   * @brief constructor
   * @param major major version
   * @param minor minor version
   * @param subminor subminor version
   * @param releaseType release type
   */
  VersionInfo(int major, int minor, int subminor, ReleaseType releaseType = RELEASE_FINAL);

  /**
   * @brief constructor
   * @param versionString the string to construct from
   **/
  VersionInfo(const QString &versionString);

  /**
   * @brief parse the version from the specified string
   *
   * @param versionString the string to parse
   **/
  void parse(const QString &versionString);

  /**
   * @return a canonicalized version string
   **/
  QString canonicalString() const;

  /**
   * @return true if this version is valid, false if it wasn't initialised or
   *         the version string was not parsable
   */
  bool isValid() const { return m_Valid; }

private:

  /**
   * @brief determine the release type
   * @param versionString the version string to parse
   * @return the version string with the release type removed
   **/
  QString parseReleaseType(QString versionString);

private:

  bool m_Valid;
  ReleaseType m_ReleaseType;
  int m_Major;
  int m_Minor;
  int m_SubMinor;

  QString m_Rest;

};


#endif // MODVERSION_H

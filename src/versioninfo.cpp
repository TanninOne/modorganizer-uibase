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


#include "versioninfo.h"
#include <QRegExp>

namespace MOBase {


VersionInfo::VersionInfo()
  : m_Valid(false), m_ReleaseType(RELEASE_FINAL), m_Major(0), m_Minor(0), m_SubMinor(0), m_Rest()
{
}


VersionInfo::VersionInfo(int major, int minor, int subminor, ReleaseType releaseType)
  : m_Valid(true), m_ReleaseType(releaseType), m_Major(major), m_Minor(minor), m_SubMinor(subminor),
    m_Rest()
{
}


VersionInfo::VersionInfo(const QString &versionString)
  : m_Valid(true), m_ReleaseType(RELEASE_FINAL),
    m_Major(0), m_Minor(0), m_SubMinor(0),
    m_Rest()
{
  parse(versionString);
}


QString VersionInfo::canonicalString() const
{
  if (!isValid()) {
    return QString();
  }
  QString result = QString("%1.%2.%3").arg(m_Major).arg(m_Minor).arg(m_SubMinor);
  switch (m_ReleaseType) {
    case RELEASE_PREALPHA: {
      result.append(" pre-alpha");
    } break;
    case RELEASE_ALPHA: {
      result.append("a");
    } break;
    case RELEASE_BETA: {
      result.append("b");
    } break;
    case RELEASE_CANDIDATE: {
      result.append("rc");
    } break;
    case RELEASE_DATE: {
      result.prepend("d");
    } break;
    default: {
      // nop
    } break;
  }

  if (!m_Rest.isEmpty()) {
    result.append(QString("%1").arg(m_Rest));
  }

  return result;
}


QString VersionInfo::parseReleaseType(QString versionString)
{
  if (versionString.contains("prealpha", Qt::CaseInsensitive)) {
    m_ReleaseType = RELEASE_PREALPHA;
    versionString.replace("prealpha", "", Qt::CaseInsensitive);
  } else if (versionString.contains("alpha", Qt::CaseInsensitive)) {
    m_ReleaseType = RELEASE_ALPHA;
    versionString.replace("alpha", "", Qt::CaseInsensitive);
  } else if (versionString.endsWith('a', Qt::CaseInsensitive)) {
    m_ReleaseType = RELEASE_ALPHA;
    versionString.chop(1);
  } else if (versionString.contains("beta", Qt::CaseInsensitive)) {
    m_ReleaseType = RELEASE_BETA;
    versionString.replace("beta", "", Qt::CaseInsensitive);
  } else if (versionString.endsWith("b", Qt::CaseInsensitive)) {
    m_ReleaseType = RELEASE_BETA;
    versionString.chop(1);
  } else if (versionString.contains("rc", Qt::CaseInsensitive)) {
    // rc is usually followed by the number of the candidate. need to extract that now, otherwise
    // the outer parser will think it's the subminor version and then 1.0.0rc1 would be interpreted as newer than 1.0.0
    int idx = versionString.indexOf("rc");
    if (idx > 1) { // make sure rc is now somehow part of the mod name
      m_Rest = versionString.mid(idx + 2);
      m_ReleaseType = RELEASE_CANDIDATE;
      versionString.remove(idx, versionString.length());
    }
  } else {
    m_ReleaseType = RELEASE_FINAL;
  }
  return versionString.trimmed();
}


void VersionInfo::parse(const QString &versionString)
{
  m_ReleaseType = RELEASE_FINAL;
  m_Major = m_Minor = m_SubMinor = 0;
  m_Rest.clear();

  if (QString::compare(versionString, "final", Qt::CaseInsensitive) == 0) {
    m_Major = 1;
    return;
  }

  QString temp = parseReleaseType(versionString);

  if (temp.startsWith('v', Qt::CaseInsensitive)) {
    temp.remove(0, 1);
  }
  if (temp.startsWith('d', Qt::CaseInsensitive) &&
      (m_ReleaseType == RELEASE_FINAL)) {
    m_ReleaseType = RELEASE_DATE;
    temp.remove(0, 1);
  }

  if (temp.length() == 0) {
    m_Valid = false;
    return;
  }

  QRegExp exp("([0-9]+)(\\.([0-9]+)(\\.([0-9]+))?)?");
  int index = exp.indexIn(temp);
  if (index > -1) {
    m_Major = exp.cap(1).toInt();
    m_Minor = exp.cap(3).toInt();
    m_SubMinor = exp.cap(5).toInt();
    // some mods use version numbering like this: 1.05 where
    // 1.1 is supposed to be higher than 1.05 (nonsense but we still need to support it. ;) )
    // In these cases we interpret the second digit as the subminor version
    if ((exp.cap(3).length() > 0) &&
        (exp.cap(5).length() == 0) &&
        (exp.cap(3).at(0) == '0')) {
      m_SubMinor = m_Minor;
      m_Minor = 0;
    }
    temp.remove(index, exp.matchedLength());
  }
  if ((m_ReleaseType == RELEASE_DATE)  && (m_Major < 1900)) {
    m_ReleaseType = RELEASE_FINAL;
  }

  m_Rest = temp.trimmed();
  m_Valid = true;
}


QDLLEXPORT bool operator<(const VersionInfo &LHS, const VersionInfo &RHS)
{
  if (!LHS.isValid() && RHS.isValid()) return true;
  if (!RHS.isValid() && LHS.isValid()) return false;

  // date-releases are lower than regular versions
  if ((LHS.m_ReleaseType == VersionInfo::RELEASE_DATE) &&
      (RHS.m_ReleaseType != VersionInfo::RELEASE_DATE)) return true;
  else if ((LHS.m_ReleaseType != VersionInfo::RELEASE_DATE) &&
           (RHS.m_ReleaseType == VersionInfo::RELEASE_DATE)) return false;

  if (LHS.m_Major != RHS.m_Major)             return LHS.m_Major < RHS.m_Major;
  if (LHS.m_Minor != RHS.m_Minor)             return LHS.m_Minor < RHS.m_Minor;
  if (LHS.m_SubMinor != RHS.m_SubMinor)       return LHS.m_SubMinor < RHS.m_SubMinor;
  if (LHS.m_ReleaseType != RHS.m_ReleaseType) return LHS.m_ReleaseType < RHS.m_ReleaseType;
  return LHS.m_Rest < RHS.m_Rest;
}


QDLLEXPORT bool operator<=(const VersionInfo &LHS, const VersionInfo &RHS)
{
  if (!LHS.isValid() && RHS.isValid()) return true;
  if (!RHS.isValid() && LHS.isValid()) return false;

  // date-releases are lower than regular versions
  if ((LHS.m_ReleaseType == VersionInfo::RELEASE_DATE) &&
      (RHS.m_ReleaseType != VersionInfo::RELEASE_DATE)) return true;
  else if ((LHS.m_ReleaseType != VersionInfo::RELEASE_DATE) &&
           (RHS.m_ReleaseType == VersionInfo::RELEASE_DATE)) return false;

  if (LHS.m_Major != RHS.m_Major)             return LHS.m_Major < RHS.m_Major;
  if (LHS.m_Minor != RHS.m_Minor)             return LHS.m_Minor < RHS.m_Minor;
  if (LHS.m_SubMinor != RHS.m_SubMinor)       return LHS.m_SubMinor < RHS.m_SubMinor;
  if (LHS.m_ReleaseType != RHS.m_ReleaseType) return LHS.m_ReleaseType < RHS.m_ReleaseType;
  return LHS.m_Rest <= RHS.m_Rest;
}


QDLLEXPORT bool operator>=(const VersionInfo &LHS, const VersionInfo &RHS)
{
  return RHS <= LHS;
}

} // namespace MOBase

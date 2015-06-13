#ifndef FILENAMESTRING_H
#define FILENAMESTRING_H

#include "dllimport.h"

#include <QString>

namespace MOBase {

/** This class wraps up a QString so the only comparisons are case insensitive
 *
 * \warn YMMV as to whether or not this is a good idea.
 * It might be better to inherit from std::string and then block the things
 * I don't need but it's hard right know to work out what those are.
 */
class FileNameString {
  friend QDLLEXPORT bool operator<(FileNameString const &lhs, FileNameString const &rhs);
  friend QDLLEXPORT bool operator==(FileNameString const &lhs, QString const &rhs);

 public:
  FileNameString()
  {}

  //Good styling says this should really be explicit, but not sure how many places
  //this would break.
  /*explicit */FileNameString(QString const &m_Name) :
    m_Name(m_Name)
  {}

  //Should be explicit but it makes initialising std::set<FileNameString> tedious
  FileNameString(char const *m_Name) :
    m_Name(m_Name)
  {}

  FileNameString(FileNameString const &other) :
    m_Name(other.m_Name)
  {}

  FileNameString &operator=(FileNameString const &other)
  {
    m_Name = other.m_Name;
    return *this;
  }

  /** Return the underlying QString. Do not overuse this! */
  QString toQString() const
  {
    return m_Name;
  }

  std::wstring toStdWString() const
  {
    return m_Name.toStdWString();
  }

  QByteArray toUtf8() const
  {
    return m_Name.toUtf8();
  }

  bool startsWith(QString const &with) const
  {
    return m_Name.startsWith(with, Qt::CaseInsensitive);
  }

  bool endsWith(QString const &with) const
  {
    return m_Name.endsWith(with, Qt::CaseInsensitive);
  }

 private:
  QString m_Name;
};

}

#endif // FILENAME_H

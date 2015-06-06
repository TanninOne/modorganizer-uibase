#include "filenamestring.h"

namespace MOBase {

bool operator<(FileNameString const &lhs, FileNameString const &rhs)
{
  //FIXME This might not be appropriate? should do a localecompare?
  return lhs.m_Name.compare(rhs.m_Name, Qt::CaseInsensitive) < 0;
}

bool operator==(FileNameString const &lhs, QString const &rhs)
{
  //FIXME This might not be appropriate? should do a localecompare?
  return lhs.m_Name.compare(rhs, Qt::CaseInsensitive) == 0;
}

}

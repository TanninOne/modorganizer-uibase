#include "imodrepositorybridge.h"
#include "json.h"

MOBase::ModRepositoryFileInfo::ModRepositoryFileInfo(const QString &data)
{
  QVariantList result = QtJson::Json::parse(data).toList();

  while (result.length() < 7) {
    result.append(QVariant());
  }
  fileID      = result.at(0).toInt();
  name        = result.at(1).toString();
  uri         = result.at(2).toString();
  version     = result.at(3).toString();
  description = result.at(4).toString();
  categoryID  = result.at(5).toInt();
  fileSize    = result.at(6).toInt();
}


QString MOBase::ModRepositoryFileInfo::toString() const
{
  return QString("[ %1,\"%2\",\"%3\",\"%4\",\"%5\",%6,%7 ]")
            .arg(fileID)
            .arg(name)
            .arg(uri)
            .arg(version.canonicalString())
            .arg(description.mid(0).replace("\"", "'"))
            .arg(categoryID)
            .arg(fileSize);
}


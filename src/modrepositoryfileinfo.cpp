#include "modrepositoryfileinfo.h"
#include "json.h"


MOBase::ModRepositoryFileInfo::ModRepositoryFileInfo(const ModRepositoryFileInfo &reference)
  : name(reference.name), uri(reference.uri), description(reference.description),
    version(reference.version), categoryID(reference.categoryID), modName(reference.modName),
    modID(reference.modID), fileID(reference.fileID), fileSize(reference.fileSize),
    fileCategory(reference.fileCategory),
    repository(reference.repository), userData(reference.userData)
{
}

MOBase::ModRepositoryFileInfo::ModRepositoryFileInfo(int modID, int fileID)
  : name(), uri(), description(), version(), categoryID(0), modName(), modID(modID), fileID(fileID),
    fileSize(0), fileCategory(TYPE_UNKNOWN), repository(), userData()

{

}


MOBase::ModRepositoryFileInfo::ModRepositoryFileInfo(const QString &data)
{
  QVariantList result = QtJson::Json::parse(data).toList();

  while (result.length() < 14) {
    result.append(QVariant());
  }
  fileID      = result.at(0).toInt();
  name        = result.at(1).toString();
  uri         = result.at(2).toString();
  version.parse(result.at(3).toString());
  description = result.at(4).toString();
  categoryID  = result.at(5).toInt();
  fileSize    = result.at(6).toInt();
  modID       = result.at(7).toInt();
  modName     = result.at(8).toString();

  newestVersion.parse(result.at(9).toString());
  fileName     = result.at(10).toString();
  fileCategory = result.at(11).toInt();
  repository   = result.at(12).toString();

  userData = result.at(13).toMap();
}


QString MOBase::ModRepositoryFileInfo::toString() const
{
  return QString("[ %1,\"%2\",\"%3\",\"%4\",\"%5\",%6,%7,%8,\"%9\",\"%10\",\"%11\",%12,\"%13\", %14 ]")
            .arg(fileID)
            .arg(name)
            .arg(uri)
            .arg(version.canonicalString())
            .arg(description.mid(0).replace("\"", "'"))
            .arg(categoryID)
            .arg(fileSize)
            .arg(modID)
            .arg(modName)
            .arg(newestVersion.canonicalString())
            .arg(fileName)
            .arg(fileCategory)
            .arg(repository)
            .arg(QString(QtJson::Json::serialize(userData)));
}

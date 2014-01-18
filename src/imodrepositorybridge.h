#ifndef INEXUSBRIDGE_H
#define INEXUSBRIDGE_H


#include <QObject>
#include <QVariant>
#include "versioninfo.h"


namespace MOBase {


class QDLLEXPORT ModRepositoryFileInfo {

public:

  ModRepositoryFileInfo() {}
  ModRepositoryFileInfo(const QString &data);
  QString toString() const;

  QString name;
  QString uri;
  QString description;
  VersionInfo version;
  int categoryID;
  int fileID;
  size_t fileSize;

};


class QDLLEXPORT IModRepositoryBridge : public QObject
{
  Q_OBJECT
public:

  IModRepositoryBridge(QObject *parent = NULL) : QObject(parent) {}

  /**
   * @brief request description for a mod
   *
   * @param modID id of the mod caller is interested in
   * @param userData user data to be returned with the result
   **/
  virtual void requestDescription(int modID, QVariant userData) = 0;

  /**
   * @brief request a list of the files belonging to a mod
   *
   * @param modID id of the mod caller is interested in
   * @param userData user data to be returned with the result
   **/
  virtual void requestFiles(int modID, QVariant userData) = 0;

  /**
   * @brief request info about a single file of a mod
   *
   * @param modID id of the mod caller is interested in
   * @param fileID id of the file the caller is interested in
   * @param userData user data to be returned with the result
   **/
  virtual void requestFileInfo(int modID, int fileID, QVariant userData) = 0;

  /**
   * @brief request the download url of a file
   *
   * @param modID id of the mod caller is interested in
   * @param fileID id of the file the caller is interested in
   * @param userData user data to be returned with the result
   **/
  virtual void requestDownloadURL(int modID, int fileID, QVariant userData) = 0;

  /**
   * @brief requestToggleEndorsement
   * @param modID id of the mod caller is interested in
   * @param userData user data to be returned with the result
   */
  virtual void requestToggleEndorsement(int modID, bool endorse, QVariant userData) = 0;

private:

  Q_DISABLE_COPY(IModRepositoryBridge)

signals:

  void descriptionAvailable(int modID, QVariant userData, QVariant resultData);
  void filesAvailable(int modID, QVariant userData, const QList<ModRepositoryFileInfo> &resultData);
  void fileInfoAvailable(int modID, int fileID, QVariant userData, QVariant resultData);
  void downloadURLsAvailable(int modID, int fileID, QVariant userData, QVariant resultData);
  void endorsementToggled(int modID, QVariant userData, QVariant resultData);
  void requestFailed(int modID, int fileID, QVariant userData, const QString &errorMessage);

};

}

#endif // INEXUSBRIDGE_H

#ifndef MODFEATURE_H
#define MODFEATURE_H


#include "modflags.h"
#include "imodinterface.h"
#include "dllimport.h"
#include <QObject>
#include <QSettings>
#include <set>


namespace MOBase {

namespace ModFeature {

class QDLLEXPORT Feature : public QObject {
  Q_OBJECT
public:
  Feature();
  void setMod(IModInterface *mod);
  virtual void saveMeta(QSettings &settings);
  virtual void readMeta(QSettings &settings);
  virtual std::set<EModFlag> flags() const;
protected:
  IModInterface *mod() const;
signals:
  void saveRequired();
private:
  IModInterface *m_Mod;
};

}

}

#endif // MODFEATURE_H


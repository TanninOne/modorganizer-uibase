#ifndef IPLUGINGAME_H
#define IPLUGINGAME_H


#include "iplugin.h"
#include <executableinfo.h>


namespace MOBase {


class IPluginGame : public QObject, public IPlugin {
  Q_INTERFACES(IPlugin)

public:

  enum LoadOrderMechanism {
    TYPE_FILETIME,
    TYPE_PLUGINSTXT
  };

public:

  IPluginGame() {}

  virtual QString gameName() const = 0;

  virtual QDir gameDirectory() const = 0;

  virtual QList<ExecutableInfo> executables() = 0;

};

} // namespace MOBase

Q_DECLARE_INTERFACE(MOBase::IPluginGame, "com.tannin.ModOrganizer.PluginGame/1.0")

#endif // IPLUGINGAME_H

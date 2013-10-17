#ifndef IPLUGINLIST_H
#define IPLUGINLIST_H


#include <QString>

namespace MOBase {

class IPluginList {

public:

  enum PluginState {
    STATE_MISSING,
    STATE_INACTIVE,
    STATE_ACTIVE
  };

public:

  virtual PluginState state(const QString &name) const = 0;
  virtual int priority(const QString &name) const = 0;
  virtual int loadOrder(const QString &name) const = 0;
  virtual bool isMaster(const QString &name) const = 0;

};

}

#endif // IPLUGINLIST_H

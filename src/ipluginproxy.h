#ifndef IPLUGINPROXY_H
#define IPLUGINPROXY_H

#include "iplugin.h"

namespace MOBase {


class IPluginProxy : public QObject, public IPlugin {
  Q_INTERFACES(IPlugin)
public:

  /**
   * @param pluginPath path to plugins
   * @return list of plugins that supported by this proxy
   */
  virtual QStringList pluginList(const QString &pluginPath) const = 0;

  /**
   * @brief instantiate a proxied plugin
   * @param pluginName name of the proxied plugin to instantiate
   * @return plugin object
   */
  virtual QObject *instantiate(const QString &pluginName) const = 0;

  /**
   * @brief sets the widget that the tool should use as the parent whenever
   *        it creates a new modal dialog
   * @param widget the new parent widget
   */
  void setParentWidget(QWidget *widget) { m_ParentWidget = widget; }

protected:

  QWidget *parentWidget() const { return m_ParentWidget; }

private:

  QWidget *m_ParentWidget;

};


} // namespace MOBase

Q_DECLARE_INTERFACE(MOBase::IPluginProxy, "com.tannin.ModOrganizer.PluginProxy/1.0")

#endif // IPLUGINPROXY_H

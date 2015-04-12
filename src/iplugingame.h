#ifndef IPLUGINGAME_H
#define IPLUGINGAME_H


#include "iplugin.h"
#include <executableinfo.h>
#include <cstdint>
#include <typeindex>
#include <unordered_map>
#include <boost/any.hpp>


namespace MOBase {


class IPluginGame : public QObject, public IPlugin {
  Q_INTERFACES(IPlugin)

public:

  enum class LoadOrderMechanism {
    FileTime,
    PluginsTxt
  };

  enum ProfileSetting {
    MODS            = 0x01,
    CONFIGURATION   = 0x02,
    SAVEGAMES       = 0x04,
    PREFER_DEFAULTS = 0x08
  };
  Q_DECLARE_FLAGS(ProfileSettings, ProfileSetting)

public:

  /**
   * @return name of the game
   */
  virtual QString gameName() const = 0;

  template <typename T>
  T *feature() {
    auto list = featureList();
    auto iter = list.find(typeid(T));
    if (iter != list.end()) {
      try {
        return boost::any_cast<T*>(iter->second);
      } catch (const boost::bad_any_cast&) {
        qCritical("failed to retrieve feature type %s (got %s)", typeid(T).name(), typeid(iter->second).name());
        return nullptr;
      }
    } else {
      return nullptr;
    }
  }

  /**
   * @brief initialize a profile for this game
   * @param directory the directory where the profile is to be initialized
   * @param settings parameters for how the profile should be initialized
   * @note the MO app does not yet support virtualizing only specific aspects but plugins should be written with this future functionality in mind
   * @note this function will be used to initially create a profile, potentially to repair it or upgrade/downgrade it so the implementations
   *       have to gracefully handle the case that the directory already contains files!
   */
  virtual void initializeProfile(const QDir &directory, ProfileSettings settings) const = 0;

  /**
   * @return file extension of save games for this game
   */
  virtual QString savegameExtension() const = 0;

  /**
   * @return true if this game has been discovered as installed, false otherwise
   */
  virtual bool isInstalled() const = 0;

  /**
   * @return an icon for this game
   */
  virtual QIcon gameIcon() const = 0;

  /**
   * @return directory to the game installation
   */
  virtual QDir gameDirectory() const = 0;

  /**
   * @brief set the path to the managed game
   * @param path to the game
   * @note this will be called by by MO to set the concrete path of the game. This is particularly
   *       relevant if the path wasn't auto-detected but had to be set manually by the user
   */
  virtual void setGamePath(const QString &path) = 0;

  /**
   * @return directory of the documents folder where configuration files and such for this game reside
   */
  virtual QDir documentsDirectory() const = 0;

  /**
   * @return path to where save games are stored.
   */
  virtual QDir savesDirectory() const = 0;

  /**
   * @return list of automatically discovered executables of the game itself and tools surrounding it
   */
  virtual QList<ExecutableInfo> executables() = 0;

  /**
   * @return steam app id for this game. Should be empty for games not available on steam
   * @note if a game is available in multiple versions those might have different app ids. the plugin should try to return the right one
   */
  virtual QString steamAPPId() const = 0;

  /**
   * @return list of plugins that are part of the game and not considered optional
   */
  virtual QStringList getPrimaryPlugins() = 0;

protected:

  virtual const std::map<std::type_index, boost::any> &featureList() const = 0;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(IPluginGame::ProfileSettings)

} // namespace MOBase

Q_DECLARE_INTERFACE(MOBase::IPluginGame, "com.tannin.ModOrganizer.PluginGame/1.0")
Q_DECLARE_METATYPE(MOBase::IPluginGame*)

#endif // IPLUGINGAME_H

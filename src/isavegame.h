#ifndef ISAVEGAMEINFO_H
#define ISAVEGAMEINFO_H

#include <QMetaType>

class QString;
class QDateTime;

namespace MOBase {

/** Base class for information about what is in a save game */
class ISaveGame
{
public:
  virtual ~ISaveGame() {}

  virtual QString getFilename() const = 0;

  virtual QDateTime getCreationTime() const = 0;

};

}

Q_DECLARE_METATYPE(MOBase::ISaveGame const *)

#endif // SAVEGAMEINFO_H


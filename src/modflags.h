#ifndef MODFLAGS
#define MODFLAGS

enum class EModFlag {
  INVALID,
  BACKUP,
  OVERWRITE,
  FOREIGN,
  NOTENDORSED,
  NOTES,
  CONFLICT_OVERWRITE,
  CONFLICT_OVERWRITTEN,
  CONFLICT_MIXED,
  CONFLICT_REDUNDANT
};

#endif // MODFLAGS

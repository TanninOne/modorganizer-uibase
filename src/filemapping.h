#ifndef FILEMAPPING_H
#define FILEMAPPING_H


#include <QString>
#include <vector>


struct Mapping {
  QString source;
  QString destination;
  bool isDirectory;
};

typedef std::vector<Mapping> MappingType;


#endif // FILEMAPPING_H

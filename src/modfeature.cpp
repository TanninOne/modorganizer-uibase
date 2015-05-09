#include "modfeature.h"


using namespace MOBase;
using namespace MOBase::ModFeature;

Feature::Feature()
  : m_Mod(nullptr)
{
}

void Feature::setMod(IModInterface *mod)
{
  m_Mod = mod;
}

void Feature::saveMeta(QSettings &settings)
{}

void Feature::readMeta(QSettings &settings)
{}

std::set<EModFlag> Feature::flags() const
{
  return std::set<EModFlag>();
}

IModInterface *Feature::mod() const
{
  return m_Mod;
}

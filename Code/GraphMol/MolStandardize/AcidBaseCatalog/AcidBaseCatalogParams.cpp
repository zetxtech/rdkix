//
//  Copyright (C) 2018-2021 Susan H. Leung and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include "AcidBaseCatalogParams.h"
#include "AcidBaseCatalogUtils.h"
#include <GraphMol/RDKixBase.h>
#include <sstream>

namespace RDKix {
namespace MolStandardize {

#include "acid_base_pairs.in"

AcidBaseCatalogParams::AcidBaseCatalogParams(const std::string &acidBaseFile) {
  d_pairs.clear();
  if (acidBaseFile.empty()) {
    d_pairs = readPairs(defaults::defaultAcidBasePairs);
  } else {
    d_pairs = readPairs(acidBaseFile);
  }
}

AcidBaseCatalogParams::AcidBaseCatalogParams(std::istream &acidBaseFile) {
  d_pairs.clear();
  d_pairs = readPairs(acidBaseFile);
}

AcidBaseCatalogParams::AcidBaseCatalogParams(
    const std::vector<std::tuple<std::string, std::string, std::string>>
        &data) {
  d_pairs.clear();
  d_pairs = readPairs(data);
}

AcidBaseCatalogParams::AcidBaseCatalogParams(
    const AcidBaseCatalogParams &other) {
  d_typeStr = other.d_typeStr;
  d_pairs.clear();

  const std::vector<std::pair<ROMOL_SPTR, ROMOL_SPTR>> &abpairs =
      other.getPairs();
  for (auto &pairi : abpairs) {
    d_pairs.emplace_back(pairi.first, pairi.second);
  }
}

AcidBaseCatalogParams::~AcidBaseCatalogParams() {}

const std::vector<std::pair<ROMOL_SPTR, ROMOL_SPTR>> &
AcidBaseCatalogParams::getPairs() const {
  return d_pairs;
}

const std::pair<ROMOL_SPTR, ROMOL_SPTR> AcidBaseCatalogParams::getPair(
    unsigned int fid) const {
  URANGE_CHECK(fid, d_pairs.size());
  return d_pairs[fid];
}

void AcidBaseCatalogParams::toStream(std::ostream &ss) const {
  ss << d_pairs.size() << "\n";
}

std::string AcidBaseCatalogParams::Serialize() const {
  std::stringstream ss;
  toStream(ss);
  return ss.str();
}

void AcidBaseCatalogParams::initFromStream(std::istream &) {
  UNDER_CONSTRUCTION("not implemented");
}

void AcidBaseCatalogParams::initFromString(const std::string &) {
  UNDER_CONSTRUCTION("not implemented");
}

}  // namespace MolStandardize
}  // namespace RDKix

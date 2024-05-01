//
//  Copyright (C) 2018-2021 Susan H. Leung and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include "TautomerCatalogParams.h"
#include "TautomerCatalogUtils.h"
#include <GraphMol/RDKixBase.h>
#include <sstream>

namespace RDKix {
namespace MolStandardize {

#include "tautomerTransforms.in"
#include "tautomerTransforms.v1.in"

TautomerCatalogParams::TautomerCatalogParams(const std::string &tautomerFile) {
  d_transforms.clear();
  if (tautomerFile.empty()) {
    d_transforms = readTautomers(defaults::defaultTautomerTransforms);
  } else {
    d_transforms = readTautomers(tautomerFile);
  }
}
TautomerCatalogParams::TautomerCatalogParams(
    const TautomerTransformDefs &data) {
  d_transforms.clear();
  d_transforms = readTautomers(data);
}

TautomerCatalogParams::TautomerCatalogParams(
    const TautomerCatalogParams &other) {
  d_typeStr = other.d_typeStr;
  d_transforms.clear();

  const std::vector<TautomerTransform> &transforms = other.getTransforms();
  for (const auto &transform : transforms) {
    d_transforms.push_back(transform);
  }
}

TautomerCatalogParams::~TautomerCatalogParams() {}

const std::vector<TautomerTransform> &TautomerCatalogParams::getTransforms()
    const {
  return d_transforms;
}

const TautomerTransform TautomerCatalogParams::getTransform(
    unsigned int fid) const {
  URANGE_CHECK(fid, d_transforms.size());
  return d_transforms[fid];  //.get();
}

void TautomerCatalogParams::toStream(std::ostream &ss) const {
  ss << d_transforms.size() << "\n";
}

std::string TautomerCatalogParams::Serialize() const {
  std::stringstream ss;
  toStream(ss);
  return ss.str();
}

void TautomerCatalogParams::initFromStream(std::istream &) {
  UNDER_CONSTRUCTION("not implemented");
}

void TautomerCatalogParams::initFromString(const std::string &) {
  UNDER_CONSTRUCTION("not implemented");
}

}  // namespace MolStandardize
}  // namespace RDKix

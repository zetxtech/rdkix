//
//  Copyright (C) 2018-2021 Susan H. Leung and other RDKix contributors
//
//   @@ All Rights Reserved @@
//  This file is part of the RDKix.
//  The contents are covered by the terms of the BSD license
//  which is included in the file license.txt, found at the root
//  of the RDKix source tree.
//
#include "TransformCatalogParams.h"
#include "TransformCatalogUtils.h"
#include <GraphMol/RDKixBase.h>
#include <sstream>

namespace RDKix {
namespace MolStandardize {

#include "normalizations.in"

TransformCatalogParams::TransformCatalogParams(
    const std::string &transformFile) {
  d_transformations.clear();
  if (transformFile.empty()) {
    d_transformations = readTransformations(defaults::defaultNormalizations);
  } else {
    d_transformations = readTransformations(transformFile);
  }
}

TransformCatalogParams::TransformCatalogParams(std::istream &transformStream) {
  d_transformations.clear();
  d_transformations = readTransformations(transformStream);
}

TransformCatalogParams::TransformCatalogParams(
    const std::vector<std::pair<std::string, std::string>> &data) {
  d_transformations.clear();
  d_transformations = readTransformations(data);
};

TransformCatalogParams::TransformCatalogParams(
    const TransformCatalogParams &other) {
  d_typeStr = other.d_typeStr;
  d_transformations.clear();

  const std::vector<std::shared_ptr<ChemicalReaction>> &otransforms =
      other.getTransformations();
  for (auto &transi : otransforms) {
    std::shared_ptr<ChemicalReaction> transform(new ChemicalReaction(*transi));
    d_transformations.push_back(transform);
  }
}

TransformCatalogParams::~TransformCatalogParams() {}

const std::vector<std::shared_ptr<ChemicalReaction>> &
TransformCatalogParams::getTransformations() const {
  return d_transformations;
}

const ChemicalReaction *TransformCatalogParams::getTransformation(
    unsigned int fid) const {
  URANGE_CHECK(fid, d_transformations.size());
  // return d_transformations[fid];
  return d_transformations[fid].get();
}

void TransformCatalogParams::initializeTransforms() const {
  for (auto &transform : d_transformations) {
    if (!transform || transform->isInitialized()) {
      continue;
    }
    transform->initReactantMatchers();
  }
}

void TransformCatalogParams::toStream(std::ostream &ss) const {
  ss << d_transformations.size() << "\n";
}

std::string TransformCatalogParams::Serialize() const {
  std::stringstream ss;
  toStream(ss);
  return ss.str();
}

void TransformCatalogParams::initFromStream(std::istream &) {
  UNDER_CONSTRUCTION("not implemented");
}

void TransformCatalogParams::initFromString(const std::string &) {
  UNDER_CONSTRUCTION("not implemented");
}

}  // namespace MolStandardize
}  // namespace RDKix
